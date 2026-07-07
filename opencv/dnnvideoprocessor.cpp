#include "dnnvideoprocessor.h"
#include "../opencv/cvconvert.h"
#include <QDebug>
#include <QThread>
#include <opencv2/imgproc.hpp>

// ══════════════════════════════════════════════════════════════════════════════
//  【DNN检测图像 → 主界面视频框】完整数据流说明
// ══════════════════════════════════════════════════════════════════════════════
//
//  本文件（DnnVideoProcessor）运行在【工作线程】中，负责读取视频帧、
//  YOLOv4-DNN 目标检测、绘制检测框，最终将处理后的图像发送到主线程，
//  再由主线程传递给 QML 界面显示。下面按步骤说明整个数据流：
//
// ┌──────────────────────────────────────────────────────────────────────────┐
// │ 步骤① 视频帧读取                    （工作线程）                         │
// │   m_capture.read(frame)  →  cv::Mat (BGR, 原始分辨率)                    │
// │                                                                          │
// │ 步骤② DNN 目标检测                  （工作线程）                         │
// │   m_detector.detect(frame, detections)                                   │
// │   → 内部流程：                                                           │
// │     a. cv::dnn::blobFromImage()   将 cv::Mat 转成 416x416 blob           │
// │     b. m_net.forward()            前向推理，输出 3 个尺度的检测结果       │
// │     c. postProcess()              解析输出 → NMS 去重 → YoloDetection[]  │
// │                                                                          │
// │ 步骤③ 绘制检测框                    （工作线程）                         │
// │   cv::rectangle() + cv::putText()  直接在 frame(cv::Mat) 上绘制          │
// │   → 绿框 + 类别名 + 置信度百分比                                         │
// │                                                                          │
// │ 步骤④ cv::Mat → QImage 转换         （工作线程）                         │
// │   QImage img = cvMatToQImage(frame);   // 定义在 opencv/cvconvert.h      │
// │   → BGR → RGB 通道交换（cv::cvtColor）                                   │
// │   → 构造 QImage，数据深拷贝（避免 cv::Mat 释放后悬空指针）               │
// │                                                                          │
// │ 步骤⑤ 跨线程信号发送                （工作线程 → 主线程）                │
// │   emit frameReady(img);                                                   │
// │   → 因为 DnnVideoProcessor 通过 moveToThread() 移到工作线程，            │
// │     而接收端（provider/controller）在主线程，Qt 自动使用                  │
// │     Qt::QueuedConnection 将 QImage 安全地传递到主线程事件队列。          │
// └──────────────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────────────┐
// │ 步骤⑥ 主线程接收 QImage             （主线程）                           │
// │   有以下几种典型方案将 QImage 交给 QML 显示：                             │
// │                                                                          │
// │   【方案A】QQuickImageProvider（推荐，本项目使用）                        │
// │     ① main.cpp 中注册：                                                  │
// │        engine.addImageProvider("dnn", new DnnImageProvider);              │
// │     ② DnnImageProvider::requestImage() 被 QML 引擎调用，                  │
// │        返回最新的 QImage（由 frameReady 信号更新缓存）                    │
// │     ③ Myvideo5.qml 中添加（不修改现有 QML）：                            │
// │        Image { source: "image://dnn/frame" }                              │
// │     ④ QML 引擎每帧自动调用 requestImage() 拉取最新图像                   │
// │                                                                          │
// │   【方案B】QQuickFramebufferObject（已在本项目视频部分使用）              │
// │     ① 自定义 DnnFboRenderer 继承 QQuickFramebufferObject::Renderer       │
// │     ② 在 render() 中将 QImage 转为 OpenGL 纹理并绘制                     │
// │     ③ QML 中以自定义 Item 形式嵌入视频区域                               │
// │                                                                          │
// │   【方案C】Q_PROPERTY 绑定                                                │
// │     ① 创建一个主线程 QObject，暴露 Q_PROPERTY(QImage frame ...)          │
// │     ② frameReady 信号连接到该对象的槽，更新 frame 属性                   │
// │     ③ QML 中 Image { source: dnnProvider.frame } 自动绑定更新            │
// └──────────────────────────────────────────────────────────────────────────┘
//
// ┌──────────────────────────────────────────────────────────────────────────┐
// │ 步骤⑦ QML 渲染显示                  （GUI 线程）                         │
// │   最终 QML 的 Image 组件（或自定义 FBO Item）将 QImage 渲染到屏幕上，    │
// │   显示在 Myvideo5.qml 的视频区域中。                                     │
// │                                                                          │
// │   注意：Myvideo5.qml 当前使用 VlcVideo 组件播放 RTSP 流，                │
// │   DNN 处理后显示需要【新增一个 Image / FBO Item】叠加或替换视频区域，    │
// │   与 VlcVideo 并存或切换。不改动 Myvideo5.qml 的前提下，可：             │
// │   - 在 Myvideo5.qml 所在的父 QML 中动态创建 Image 覆盖层                 │
// │   - 或通过 Loader 按模式切换 VlcVideo / DNN Image                        │
// └──────────────────────────────────────────────────────────────────────────┘
//
//  关键信号链路总结：
//  processFrame() [工作线程]
//       │
//       ├─ emit frameReady(QImage) ──→ 主线程槽函数（QueuedConnection）
//       │                                    │
//       │                                    ├─ 更新 ImageProvider 缓存
//       │                                    ├─ 或 更新 Q_PROPERTY(QImage)
//       │                                    └─ 或 触发 FBO update()
//       │
//       └─ emit detectionsReady(vector<YoloDetection>)  （可选：检测数据）
//
// ══════════════════════════════════════════════════════════════════════════════

// ========== 构造 / 析构 ==========
DnnVideoProcessor::DnnVideoProcessor(QObject *parent)
    : QObject(parent), m_running(false)
{
}

DnnVideoProcessor::~DnnVideoProcessor()
{
    stop();
}

// ========== 参数 ==========
void DnnVideoProcessor::setTargetFps(int fps)
{
    if (fps > 0 && fps <= 60)
        m_targetFps = fps;
}

void DnnVideoProcessor::setConfThreshold(float t) { m_detector.setConfThreshold(t); }
void DnnVideoProcessor::setNmsThreshold(float t)  { m_detector.setNmsThreshold(t); }

void DnnVideoProcessor::setInputSize(int width, int height)
{
    m_detector.setInputSize(width, height);
}

void DnnVideoProcessor::setBackend(int backend, int target)
{
    m_detector.setBackend(backend, target);
}

// ========== 初始化 ==========
bool DnnVideoProcessor::openVideo(const QString &videoPath)
{
    m_capture.open(videoPath.toLocal8Bit().constData());
    if (!m_capture.isOpened()) {
        emit error("Cannot open video: " + videoPath);
        return false;
    }
    qDebug() << "[DnnVideoProcessor] Video opened:" << videoPath;
    return true;
}

bool DnnVideoProcessor::loadYoloModel(const QString &cfgPath,
                                      const QString &weightsPath,
                                      const QString &namesPath)
{
    // 1. 加载模型
    if (!m_detector.loadModel(cfgPath.toStdString(), weightsPath.toStdString())) {
        emit error("Failed to load YOLO model:\n  cfg: " + cfgPath +
                    "\n  weights: " + weightsPath);
        return false;
    }

    // 2. 加载类别名称
    if (!namesPath.isEmpty()) {
        if (!m_detector.loadClassNames(namesPath.toStdString())) {
            qWarning() << "[DnnVideoProcessor] Class names file not loaded:"
                       << namesPath << "(detection will show class index only)";
        }
    }

    qDebug() << "[DnnVideoProcessor] YOLOv4 model ready,"
             << "classes:" << m_detector.classCount()
             << "input size:" << 416 << "x" << 416;
    return true;
}

// ========== 运行时 ==========
void DnnVideoProcessor::start()
{
    m_running = true;

    if (!m_timer) {
        m_timer = new QTimer(this);
        m_timer->setSingleShot(true);
        QObject::connect(m_timer, &QTimer::timeout,
                         this, &DnnVideoProcessor::processFrame);
    }

    qDebug() << "[DnnVideoProcessor] Started, thread:" << QThread::currentThread();
    processFrame();
}

void DnnVideoProcessor::stop()
{
    m_running = false;
    if (m_timer)
        m_timer->stop();
}

void DnnVideoProcessor::processFrame()
{
    if (!m_running) {
        qDebug() << "[DnnVideoProcessor] Stopped";
        emit finished();
        return;
    }

    // ── 1. 读取一帧 ──
    cv::Mat frame;
    if (!m_capture.read(frame)) {
        qDebug() << "[DnnVideoProcessor] Video ended";
        emit finished();
        return;
    }

    // ── 2. DNN 目标检测 ──
    std::vector<YoloDetection> detections;
    m_detector.detect(frame, detections);

    // ── 3. 绘制检测框（步骤③：直接在 cv::Mat 上绘制绿框+标签）──
    if (m_drawBoxes) {
        for (const auto &det : detections) {
            // 绘制边界框
            cv::rectangle(frame, det.bbox, cv::Scalar(0, 255, 0), 2);

            // 绘制标签背景和文字
            std::string label = det.className
                                + " " + std::to_string(int(det.confidence * 100)) + "%";

            int baseline = 0;
            cv::Size labelSize = cv::getTextSize(label,
                                                  cv::FONT_HERSHEY_SIMPLEX,
                                                  0.5, 1, &baseline);

            cv::Rect labelRect(det.bbox.x, det.bbox.y - labelSize.height - 5,
                               labelSize.width, labelSize.height + 5);
            // 防止标签超出图像顶部
            if (labelRect.y < 0) {
                labelRect.y = det.bbox.y + det.bbox.height + 5;
            }

            cv::rectangle(frame, labelRect, cv::Scalar(0, 255, 0), cv::FILLED);
            cv::putText(frame, label,
                        cv::Point(labelRect.x, labelRect.y + labelSize.height),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5,
                        cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
        }
    }

    // ── 4. cv::Mat → QImage 转换（步骤④）──
    //     在 cvconvert.h 中定义：BGR→RGB 通道交换 + 深拷贝构造 QImage
    QImage img = cvMatToQImage(frame);
    if (!img.isNull()) {
        // emit 跨线程发送（步骤⑤）：QML 引擎的 ImageProvider/FBO 在主线程接收
        emit frameReady(img);
    }

    // 同时发送检测结果（步骤⑤补充：结构化检测数据，可选用于 QML 标签列表）
    if (!detections.empty()) {
        emit detectionsReady(detections);
    }

    // ── 5. 按目标帧率调度下一帧 ──
    int interval = 1000 / m_targetFps;
    if (interval < 5)
        interval = 5;

    m_timer->start(interval);
}
