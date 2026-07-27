// streamprocessor.cpp — 从 VlcVideoItem 取帧 → ONNX 检测 → 回传显示
//
// ══════════════════════════════════════════════════════════════════════════════
//  数据流
// ══════════════════════════════════════════════════════════════════════════════
//
//  本文件运行在【工作线程】中。
//  不拉流、不解码——帧来源和最终显示均由 VlcVideoItem 负责。
//
// ┌──────────────────────────────────────────────────────────────────────────┐
// │ 步骤① 取帧                             （工作线程 / QTimer）             │
// │   m_videoSource->grabFrame()                                             │
// │   → libvlc 最新解码帧的深拷贝（RGBA QImage，线程安全）                    │
// │                                                                          │
// │ 步骤② QImage → cv::Mat 转换            （工作线程）                       │
// │   cv::Mat rgbaMat → cv::cvtColor(RGBA2BGR) → cv::Mat frame (BGR)        │
// │                                                                          │
// │ 步骤③ 基础图像处理【可选】             （工作线程）                       │
// │   applyImageProcessing(frame)                                            │
// │                                                                          │
// │ 步骤④ ONNX YOLOv3-tiny 推理            （工作线程）                       │
// │   m_detector.detect(frame, detections)                                    │
// │                                                                          │
// │ 步骤⑤ 绘制检测框                       （工作线程）                       │
// │   drawDetections(frame, detections)                                       │
// │                                                                          │
// │ 步骤⑥ 回传 + 广播                      （工作线程 → 主线程）              │
// │   cv::Mat → QImage(RGB) 转换                                             │
// │   m_videoSource->submitProcessedFrame(img)  — 回传 VlcVideoItem 显示      │
// │   emit frameReady(img)                     — 同时对外广播                 │
// └──────────────────────────────────────────────────────────────────────────┘
//
// ══════════════════════════════════════════════════════════════════════════════

#include "streamprocessor.h"
#include "cvconvert.h"
#include "VlcVideoItem.h"
#include <QDebug>
#include <QElapsedTimer>
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
// 视频源
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::setVideoSource(VlcVideoItem *source) {
    m_videoSource = source;
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
// 模型加载
// ══════════════════════════════════════════════════════════════════════════════

bool StreamProcessor::loadYoloModel(const QString &onnxPath,
                                    const QString &namesPath) {
    if (!m_detector.loadModel(onnxPath.toStdString())) {
        emit errorOccurred(QString("Failed to load ONNX model: %1").arg(onnxPath));
        return false;
    }

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
// 基础图像处理
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::applyImageProcessing(cv::Mat &frame) {
    if (frame.empty()) return;

    if (m_denoise) {
        cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0.5);
    }

    if (m_autoEnhance) {
        cv::Mat lab;
        cv::cvtColor(frame, lab, cv::COLOR_BGR2Lab);
        std::vector<cv::Mat> labChannels(3);
        cv::split(lab, labChannels);
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
        clahe->apply(labChannels[0], labChannels[0]);
        cv::merge(labChannels, lab);
        cv::cvtColor(lab, frame, cv::COLOR_Lab2BGR);
    }

    if (m_histEq) {
        cv::Mat yuv;
        cv::cvtColor(frame, yuv, cv::COLOR_BGR2YUV);
        std::vector<cv::Mat> yuvChannels(3);
        cv::split(yuv, yuvChannels);
        cv::equalizeHist(yuvChannels[0], yuvChannels[0]);
        cv::merge(yuvChannels, yuv);
        cv::cvtColor(yuv, frame, cv::COLOR_YUV2BGR);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// 绘制检测框
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::drawDetections(
    cv::Mat &frame,
    const std::vector<OnnxDetection> &detections)
{
    // 找置信度最高的检测框，记录其中心坐标
    const OnnxDetection *bestDet = nullptr;
    float bestConf = 0.0f;

    for (const auto &det : detections) {
        cv::rectangle(frame, det.bbox, m_boxColor, 2);

        std::string label = det.className
                            + " " + std::to_string(static_cast<int>(det.confidence * 100)) + "%";

        int baseline = 0;
        double fontScale = 0.5;
        int thickness = 1;
        cv::Size labelSize = cv::getTextSize(
            label, cv::FONT_HERSHEY_SIMPLEX,
            fontScale, thickness, &baseline);

        int labelY = det.bbox.y - labelSize.height - 5;
        if (labelY < 0)
            labelY = det.bbox.y + det.bbox.height + 5;

        cv::Rect labelRect(det.bbox.x, labelY,
                          labelSize.width, labelSize.height + 5);
        if (labelRect.x + labelRect.width > frame.cols)
            labelRect.x = frame.cols - labelRect.width - 2;
        if (labelRect.x < 0)
            labelRect.x = 2;

        cv::rectangle(frame, labelRect, m_boxColor, cv::FILLED);
        cv::putText(frame, label,
                    cv::Point(labelRect.x, labelRect.y + labelSize.height),
                    cv::FONT_HERSHEY_SIMPLEX, fontScale,
                    cv::Scalar(0, 0, 0), thickness, cv::LINE_AA);

        // 追踪最高置信度目标
        if (det.confidence > bestConf) {
            bestConf = det.confidence;
            bestDet = &det;
        }
    }

    // 存储最高置信度检测框的中心像素坐标
    if (bestDet) {
        m_centerX = bestDet->bbox.x + bestDet->bbox.width / 2;
        m_centerY = bestDet->bbox.y + bestDet->bbox.height / 2;
    } else {
        m_centerX = -1;
        m_centerY = -1;
    }
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
//
// QTimer 驱动，工作线程中执行。
// 从 VlcVideoItem 取最新解码帧 → OpenCV 处理 → 回传 VlcVideoItem 显示。
// ══════════════════════════════════════════════════════════════════════════════

void StreamProcessor::processFrame() {
    
    if (!m_running) {
        qDebug() << "[StreamProcessor] Stopped";
        emit finished();
        return;
    }

    // 固定目标帧间隔（ms）
    const int targetInterval = std::max(5, 1000 / m_targetFps);
    // ── 帧率控制起点：记录本次处理开始时间 ──
    QElapsedTimer frameTimer;
    frameTimer.start();

    // ── ① 从 VlcVideoItem 取帧 ──
    cv::Mat frame;
    if (m_videoSource) {
        //跨线程拷贝有一定开销，后续可优化为拿共享指针
        QImage rawFrame = m_videoSource->grabFrame();   // 线程安全深拷贝
        if (!rawFrame.isNull()) {
            // QImage(RGBA) → cv::Mat(BGR)
            cv::Mat rgbaMat(rawFrame.height(), rawFrame.width(),
                            CV_8UC4, const_cast<uchar*>(rawFrame.bits()),
                            static_cast<size_t>(rawFrame.bytesPerLine()));
            cv::cvtColor(rgbaMat, frame, cv::COLOR_RGBA2BGR);
            // frame 是独立的 BGR cv::Mat，rawFrame 可在此后释放
        }
    }

    if (frame.empty()) {
        // 没有可用帧
        m_timer->start(5);      // 快速重试
        return;
    }

    // ── ② 基础图像处理 ──
    applyImageProcessing(frame);

    // ── ③ ONNX YOLOv3-tiny 目标检测 ──
    std::vector<OnnxDetection> detections;
    if (isModelLoaded()) {
        m_detector.detect(frame, detections);
    }

    // ── 提取最高置信度检测框中心（原始测量值，供卡尔曼滤波用）──
    float rawCenterX = -1.0f;
    float rawCenterY = -1.0f;
    const OnnxDetection *bestDet = nullptr;
    if (!detections.empty()) {
        float bestConf = 0.0f;
        for (const auto &det : detections) {
            if (det.confidence > bestConf) {
                bestConf = det.confidence;
                bestDet  = &det;
            }
        }
        if (bestDet) {
            rawCenterX = static_cast<float>(bestDet->bbox.x
                                            + bestDet->bbox.width  / 2);
            rawCenterY = static_cast<float>(bestDet->bbox.y
                                            + bestDet->bbox.height / 2);
        }
    }

    // ── ④ 绘制检测框 ──
    if (m_drawBoxes && !detections.empty()) {
        if (m_trackSingleTarget && bestDet) {
            // 单目标模式：只画最高置信度目标
            drawDetections(frame, {*bestDet});
        } else {
            // 多目标模式：画全部检测结果
            drawDetections(frame, detections);
        }
    }

    // ── ④½ 卡尔曼滤波：消除检测框抖动，输出平滑坐标 ──
    {
        float dt = 0.0f;
        if (m_kalmanFirstFrame) {
            m_kalmanTimer.start();
            m_kalmanFirstFrame = false;
        } else {
            dt = static_cast<float>(m_kalmanTimer.restart()) / 1000.0f;
        }
        m_tracker.feed(rawCenterX, rawCenterY, dt);
        m_centerX = static_cast<int>(m_tracker.filteredX());
        m_centerY = static_cast<int>(m_tracker.filteredY());
    }

    // ── ⑤ 显示缩放（仅影响最终输出，不改变 ONNX 输入分辨率）──
    if (m_displayWidth > 0 && m_displayHeight > 0) {
        cv::resize(frame, frame, cv::Size(m_displayWidth, m_displayHeight),
                   0, 0, cv::INTER_LINEAR);
    }

    // ── ⑥ cv::Mat → QImage 转换 ──
    QImage img = cvMatToQImage(frame);

    // ── ⑦ 回传 VlcVideoItem 显示 + 对外广播 ──
    if (!img.isNull()) {
        if (m_videoSource) {
            m_videoSource->submitProcessedFrame(img);
        }
        emit frameReady(img);
    }

    if (!detections.empty()) {
        emit detectionsReady(detections);
    }

    // ── ⑧ 帧率控制：按实际耗时动态调节 ──
    //  耗时 < 目标间隔 → 等剩余时间，精准控帧
    //  耗时 ≥ 目标间隔 → 立即下一帧，不积压
    
    int elapsed  = static_cast<int>(frameTimer.elapsed());
    int remaining = targetInterval - elapsed;

    if (remaining > 0) {
        m_timer->start(remaining);
    } else {
        m_timer->start(0);  // 已超时，立即进入下一轮
    }
}
