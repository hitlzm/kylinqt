// streamprocessor.cpp — RTSP 拉流 + ONNX YOLOv3-tiny 检测 + 显示
//
// ══════════════════════════════════════════════════════════════════════════════
//  【RTSP 流 → YOLO ONNX 检测 → 界面显示】完整数据流
// ══════════════════════════════════════════════════════════════════════════════
//
//  本文件运行在【工作线程】中，负责拉流、检测、绘制、转码，
//  最终通过信号将 QImage 发送到主线程交给 QML 显示。
//
// ┌──────────────────────────────────────────────────────────────────────────┐
// │ 步骤① RTSP 拉流                      （工作线程）                        │
// │   cv::VideoCapture 通过 FFmpeg 后端打开 RTSP/RTMP 流                     │
// │   m_capture.read(frame)  →  cv::Mat (BGR, 原始分辨率)                    │
// │                                                                          │
// │ 步骤② 基础图像处理【可选】           （工作线程）                        │
// │   applyImageProcessing(frame):                                            │
// │     - 自动增强：CLAHE + 亮度对比度调整                                    │
// │     - 直方图均衡化：equalizeHist()                                       │
// │     - 高斯降噪：GaussianBlur()                                           │
// │     - 显示缩放：resize()                                                 │
// │                                                                          │
// │ 步骤③ ONNX YOLOv3-tiny 推理           （工作线程）                        │
// │   m_detector.detect(frame, detections)                                    │
// │   → 内部流程：                                                            │
// │     a. preprocessFrame(): BGR→blob (resize+normalize+NCHW)               │
// │     b. session->Run(): ONNX Runtime 前向推理                              │
// │     c. decodeYoloScale(): 双尺度解码 (13×13 + 26×26)                     │
// │     d. NMSBoxes(): 非极大值抑制                                          │
// │                                                                          │
// │ 步骤④ 绘制检测框                      （工作线程）                        │
// │   drawDetections(frame, detections):                                      │
// │     - cv::rectangle()  绿色/自定义颜色边界框                              │
// │     - cv::putText()    类别名 + 置信度百分比                              │
// │     - 标签背景填充                                                        │
// │                                                                          │
// │ 步骤⑤ cv::Mat → QImage 转换           （工作线程）                        │
// │   cvMatToQImage(frame)  — 定义在 opencv/cvconvert.h                      │
// │   → BGR→RGB 通道交换（cv::cvtColor SIMD 加速）                           │
// │                                                                          │
// │ 步骤⑥ 跨线程信号发送                  （工作线程 → 主线程）               │
// │   emit frameReady(img)                                                    │
// │   → Qt::QueuedConnection 将 QImage 安全传递到主线程事件队列              │
// │   → 主线程 ImageProvider / FBO / Q_PROPERTY 接收并交给 QML 渲染          │
// │                                                                          │
// │ 步骤⑦ 断流重连                        （工作线程）                        │
// │   当 m_capture.read() 失败时：                                            │
// │     - 释放当前 VideoCapture                                              │
// │     - 等待 m_reconnectMs 毫秒后重新打开流                                 │
// │     - 最多重试无限次（指数退避最大 30 秒）                                │
// └──────────────────────────────────────────────────────────────────────────┘
//
// ══════════════════════════════════════════════════════════════════════════════

#include "streamprocessor.h"
#include "cvconvert.h"
#include <QDebug>
#include <QThread>
#include <opencv2/imgproc.hpp>

// ══════════════════════════════════════════════════════════════════════════════
// 构造 / 析构
// ══════════════════════════════════════════════════════════════════════════════

StreamProcessor::StreamProcessor(QObject *parent)
    : QObject(parent)
    , m_running(false)
{
}

StreamProcessor::~StreamProcessor() {
    stop();
}

// ══════════════════════════════════════════════════════════════════════════════
// 参数设置
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::setTargetFps(int fps) {
    if (fps > 0 && fps <= 60)
        m_targetFps = fps;
}

void StreamProcessor::setConfThreshold(float t) { m_detector.setConfThreshold(t); }
void StreamProcessor::setNmsThreshold(float t)  { m_detector.setNmsThreshold(t); }

void StreamProcessor::setInputSize(int width, int height) {
    m_detector.setInputSize(width, height);
}

// ══════════════════════════════════════════════════════════════════════════════
// 初始化 — 打开 RTSP 流
// ══════════════════════════════════════════════════════════════════════════════

bool StreamProcessor::openStream(const QString &streamUrl, bool tcpTransport) {
    m_streamUrl = streamUrl;
    m_tcpTransport = tcpTransport;

    // 释放之前的捕获
    if (m_capture.isOpened())
        m_capture.release();

    // ── 配置 FFmpeg 后端参数 ──
    // 使用 TCP 传输可避免 UDP 丢包导致的画面花屏，但延迟略高
    if (m_tcpTransport) {
        // 设置环境变量让 FFmpeg 使用 TCP
        _putenv("OPENCV_FFMPEG_CAPTURE_OPTIONS=rtsp_transport;tcp");
    }

    // 打开流（cv::VideoCapture 内部调用 FFmpeg）
    // 对于 RTSP 流，通常需要指定 cv::CAP_FFMPEG 后端
    bool ok = false;
    try {
        ok = m_capture.open(m_streamUrl.toLocal8Bit().constData(),
                            cv::CAP_FFMPEG);
    } catch (const cv::Exception &e) {
        qWarning() << "[StreamProcessor] cv::VideoCapture exception:" << e.what();
        ok = false;
    }

    if (!ok) {
        // 尝试不指定后端（让 OpenCV 自动选择）
        ok = m_capture.open(m_streamUrl.toLocal8Bit().constData());
    }

    if (!ok) {
        emit errorOccurred(QString("Cannot open stream: %1").arg(m_streamUrl));
        m_streamOpened = false;
        return false;
    }

    // ── 配置捕获参数 ──
    // 设置缓冲区大小（减少延迟）
    m_capture.set(cv::CAP_PROP_BUFFERSIZE, 3);
    // 某些流需要设置帧率
    // m_capture.set(cv::CAP_PROP_FPS, m_targetFps);

    m_streamOpened = true;
    m_reconnectCount = 0;

    qDebug() << "[StreamProcessor] Stream opened:" << m_streamUrl
             << "resolution:" << m_capture.get(cv::CAP_PROP_FRAME_WIDTH) << "x"
             << m_capture.get(cv::CAP_PROP_FRAME_HEIGHT)
             << "fps:" << m_capture.get(cv::CAP_PROP_FPS)
             << "transport:" << (m_tcpTransport ? "TCP" : "UDP");

    emit streamStatusChanged(true);
    return true;
}

bool StreamProcessor::loadYoloModel(const QString &onnxPath,
                                    const QString &namesPath) {
    // 1. 加载 ONNX 模型
    if (!m_detector.loadModel(onnxPath.toStdString())) {
        emit errorOccurred(QString("Failed to load ONNX model: %1").arg(onnxPath));
        return false;
    }

    // 2. 加载类别名称
    if (!namesPath.isEmpty()) {
        if (!m_detector.loadClassNames(namesPath.toStdString())) {
            qWarning() << "[StreamProcessor] Class names file not loaded:"
                       << namesPath << "(will show class index instead)";
        }
    }

    qDebug() << "[StreamProcessor] YOLOv3-tiny ONNX model ready,"
             << "classes:" << m_detector.classCount()
             << "input:" << 416 << "x" << 416;
    return true;
}

// ══════════════════════════════════════════════════════════════════════════════
// 基础图像处理（步骤②）
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::applyImageProcessing(cv::Mat &frame) {
    if (frame.empty()) return;

    // ── 1. 降噪（先于增强，避免放大噪声）──
    if (m_denoise) {
        // 轻度高斯模糊去噪（保留边缘）
        cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0.5);
    }

    // ── 2. 自动增强（CLAHE + 亮度/对比度）──
    if (m_autoEnhance) {
        // 转为 LAB 色彩空间，仅对 L 通道做 CLAHE
        cv::Mat lab;
        cv::cvtColor(frame, lab, cv::COLOR_BGR2Lab);

        std::vector<cv::Mat> labChannels(3);
        cv::split(lab, labChannels);

        // CLAHE: 对比度受限自适应直方图均衡
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
        clahe->apply(labChannels[0], labChannels[0]);

        cv::merge(labChannels, lab);
        cv::cvtColor(lab, frame, cv::COLOR_Lab2BGR);
    }

    // ── 3. 直方图均衡化 ──
    if (m_histEq) {
        // 转为 YUV，仅均衡 Y 通道（亮度）
        cv::Mat yuv;
        cv::cvtColor(frame, yuv, cv::COLOR_BGR2YUV);

        std::vector<cv::Mat> yuvChannels(3);
        cv::split(yuv, yuvChannels);
        cv::equalizeHist(yuvChannels[0], yuvChannels[0]);
        cv::merge(yuvChannels, yuv);

        cv::cvtColor(yuv, frame, cv::COLOR_YUV2BGR);
    }

    // ── 4. 显示缩放 ──
    if (m_displayWidth > 0 && m_displayHeight > 0) {
        cv::resize(frame, frame, cv::Size(m_displayWidth, m_displayHeight),
                   0, 0, cv::INTER_LINEAR);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// 绘制检测框（步骤④）
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::drawDetections(
    cv::Mat &frame,
    const std::vector<OnnxDetection> &detections)
{
    for (const auto &det : detections) {
        // ── 边界框 ──
        cv::rectangle(frame, det.bbox, m_boxColor, 2);

        // ── 标签文字 ──
        std::string label = det.className
                            + " " + std::to_string(static_cast<int>(det.confidence * 100)) + "%";

        int baseline = 0;
        double fontScale = 0.5;
        int thickness = 1;

        cv::Size labelSize = cv::getTextSize(
            label, cv::FONT_HERSHEY_SIMPLEX,
            fontScale, thickness, &baseline);

        // 标签位置（默认在框上方，若超出图像则放在框下方）
        int labelY = det.bbox.y - labelSize.height - 5;
        if (labelY < 0)
            labelY = det.bbox.y + det.bbox.height + 5;

        cv::Rect labelRect(det.bbox.x, labelY,
                          labelSize.width, labelSize.height + 5);

        // 标签越界修正
        if (labelRect.x + labelRect.width > frame.cols)
            labelRect.x = frame.cols - labelRect.width - 2;
        if (labelRect.x < 0)
            labelRect.x = 2;

        // ── 标签背景填充 ──
        cv::rectangle(frame, labelRect, m_boxColor, cv::FILLED);

        // ── 标签文字（黑色，绘制在背景上方）──
        cv::putText(frame, label,
                    cv::Point(labelRect.x, labelRect.y + labelSize.height),
                    cv::FONT_HERSHEY_SIMPLEX, fontScale,
                    cv::Scalar(0, 0, 0), thickness, cv::LINE_AA);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// 断流重连
//
// 采用指数退避策略：首次等待 m_reconnectMs，后续每次翻倍，
// 最大等待 30 秒。重连成功后重置计数器。
// ══════════════════════════════════════════════════════════════════════════════

bool StreamProcessor::tryReconnect() {
    m_reconnectCount++;

    // 指数退避：3s → 6s → 12s → 24s → 30s(max)
    int delay = m_reconnectMs * (1 << std::min(m_reconnectCount - 1, 3));
    if (delay > 30000) delay = 30000;

    qDebug() << "[StreamProcessor] Reconnecting in" << delay
             << "ms (attempt" << m_reconnectCount << ")...";

    // 同步等待（processFrame 返回后 QTimer 会再次触发）
    QThread::msleep(delay);

    // 释放旧捕获
    if (m_capture.isOpened())
        m_capture.release();

    m_streamOpened = false;

    // 尝试重新打开
    if (m_tcpTransport) {
        _putenv("OPENCV_FFMPEG_CAPTURE_OPTIONS=rtsp_transport;tcp");
    }

    bool ok = m_capture.open(m_streamUrl.toLocal8Bit().constData(), cv::CAP_FFMPEG);
    if (!ok) {
        ok = m_capture.open(m_streamUrl.toLocal8Bit().constData());
    }

    if (ok) {
        m_capture.set(cv::CAP_PROP_BUFFERSIZE, 3);
        m_streamOpened = true;
        m_reconnectCount = 0;
        qDebug() << "[StreamProcessor] Reconnected successfully";
        emit streamStatusChanged(true);
        return true;
    }

    qWarning() << "[StreamProcessor] Reconnect failed, will retry...";
    emit streamStatusChanged(false);
    return false;
}

// ══════════════════════════════════════════════════════════════════════════════
// 运行时控制
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::start() {
    m_running = true;

    if (!m_timer) {
        m_timer = new QTimer(this);
        m_timer->setSingleShot(true);
        QObject::connect(m_timer, &QTimer::timeout,
                         this, &StreamProcessor::processFrame);
    }

    qDebug() << "[StreamProcessor] Started, thread:" << QThread::currentThread();
    processFrame();
}

void StreamProcessor::stop() {
    m_running = false;
    if (m_timer)
        m_timer->stop();
}

// ══════════════════════════════════════════════════════════════════════════════
// 核心帧处理循环
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::processFrame() {
    if (!m_running) {
        qDebug() << "[StreamProcessor] Stopped";
        if (m_capture.isOpened())
            m_capture.release();
        m_streamOpened = false;
        emit finished();
        return;
    }

    // ── 0. 检查流状态 ──
    if (!m_streamOpened || !m_capture.isOpened()) {
        if (!m_streamUrl.isEmpty()) {
            // 尝试重连
            if (!tryReconnect()) {
                // 重连失败，按目标帧率调度下一次重连尝试
                int interval = 1000 / m_targetFps;
                if (interval < 50) interval = 50;
                m_timer->start(interval);
                return;
            }
        } else {
            // 没有流地址，跳过这一帧
            int interval = 1000 / m_targetFps;
            m_timer->start(interval);
            return;
        }
    }

    // ── 1. 读取一帧 ──
    cv::Mat frame;
    bool readOk = false;
    try {
        readOk = m_capture.read(frame);
    } catch (const cv::Exception &e) {
        qWarning() << "[StreamProcessor] Read exception:" << e.what();
        readOk = false;
    }

    if (!readOk || frame.empty()) {
        qWarning() << "[StreamProcessor] Frame read failed, stream may be disconnected";
        m_streamOpened = false;
        m_capture.release();
        emit streamStatusChanged(false);

        // 调度重连
        int interval = 1000 / m_targetFps;
        if (interval < 50) interval = 50;
        m_timer->start(interval);
        return;
    }

    // ── 2. 基础图像处理 ──
    applyImageProcessing(frame);

    // ── 3. ONNX YOLOv3-tiny 目标检测 ──
    std::vector<OnnxDetection> detections;
    if (isModelLoaded()) {
        m_detector.detect(frame, detections);
    }

    // ── 4. 绘制检测框 ──
    if (m_drawBoxes && !detections.empty()) {
        drawDetections(frame, detections);
    }

    // ── 5. cv::Mat → QImage 转换 ──
    QImage img = cvMatToQImage(frame);
    if (!img.isNull()) {
        emit frameReady(img);
    }

    // ── 6. 发送检测数据（可选）──
    if (!detections.empty()) {
        emit detectionsReady(detections);
    }

    // ── 7. 按目标帧率调度下一帧 ──
    int interval = 1000 / m_targetFps;
    if (interval < 5)
        interval = 5;
    m_timer->start(interval);
}
