// streamprocessor.h — RTSP 拉流 + ONNX YOLOv3-tiny 检测 + 显示的视频处理器
//
// 与 DnnVideoProcessor（本地文件 + OpenCV DNN YOLOv4）对称设计，
// 可在 main.cpp 中按需替换。内部持有 OnnxYoloDetector 实例，
// 通过 QTimer 驱动逐帧处理，检测结果绘制到图像上后以 QImage
// 形式发送到主线程显示。
//
// 主要差异：
//  - 使用 ONNX Runtime 推理（OnnxYoloDetector）而非 OpenCV DNN
//  - 支持 RTSP/RTMP 拉流（cv::VideoCapture + FFmpeg 后端）
//  - 内置断流重连机制
//  - 可选基础图像预处理（亮度/对比度/直方图均衡/降噪）
//
// 典型用法：
// @code
//   QThread *thread = new QThread;
//   StreamProcessor *proc = new StreamProcessor;
//   proc->openStream("rtsp://192.168.1.100:554/stream");
//   proc->loadYoloModel("yolov3-tiny.onnx", "coco.names");
//   proc->moveToThread(thread);
//   // 连接信号...
//   QObject::connect(thread, &QThread::started, proc, &StreamProcessor::start);
//   QObject::connect(proc, &StreamProcessor::frameReady, provider, &XxxProvider::onFrame);
//   thread->start();
// @endcode
//
// 显示方案（参考 DnnVideoProcessor 中的详细说明）：
//   A. QQuickImageProvider — 推荐，QML Image { source: "image://stream/frame" }
//   B. QQuickFramebufferObject — 类似 VlcVideoItem 的 OpenGL 渲染
//   C. Q_PROPERTY(QImage frame ...) — QML 属性绑定

#ifndef STREAMPROCESSOR_H
#define STREAMPROCESSOR_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <atomic>
#include <opencv2/videoio.hpp>
#include "onnxyolodetector.h"

/**
 * @brief 视频流读取 + ONNX YOLOv3-tiny 目标检测工作线程
 *
 * 运行在独立 QThread 中，负责：
 *  1. 从 RTSP/RTMP 流拉取视频帧
 *  2. 可选的基础图像处理（去噪、增强等）
 *  3. YOLOv3-tiny ONNX 推理
 *  4. 绘制检测框和标签
 *  5. cv::Mat → QImage 转换
 *  6. 通过信号将处理后的帧发送到主线程
 */
class StreamProcessor : public QObject
{
    Q_OBJECT
public:
    explicit StreamProcessor(QObject *parent = nullptr);
    ~StreamProcessor() override;

    // ── 初始化 ────────────────────────────────────────────
    /**
     * @brief 打开 RTSP/RTMP 视频流
     * @param streamUrl  流地址（rtsp://... 或 rtmp://...）
     * @param tcpTransport 是否使用 TCP 传输（更稳定，默认 true）
     * @return true 打开成功
     */
    bool openStream(const QString &streamUrl, bool tcpTransport = true);

    /**
     * @brief 一键加载 YOLOv3-tiny ONNX 模型 + 类别名称
     * @param onnxPath   .onnx 模型文件路径
     * @param namesPath  类别名称文件路径（如 coco.names）
     * @return true 全部加载成功
     */
    bool loadYoloModel(const QString &onnxPath,
                       const QString &namesPath = QString());

    // ── 参数调节 ──────────────────────────────────────────
    void setTargetFps(int fps);                         // 目标帧率，默认 25
    void setConfThreshold(float t);                     // 置信度阈值，默认 0.5
    void setNmsThreshold(float t);                      // NMS IoU 阈值，默认 0.4
    void setInputSize(int width, int height);           // 网络输入尺寸，默认 416×416

    /// 是否绘制检测框和标签（默认 true）
    void setDrawBoxes(bool draw) { m_drawBoxes = draw; }

    /// 设置检测框颜色（BGR），默认绿色 (0, 255, 0)
    void setBoxColor(int b, int g, int r) {
        m_boxColor = cv::Scalar(b, g, r);
    }

    // ── 基础图像处理开关 ─────────────────────────────────
    /// 启用自动白平衡/亮度对比度调整
    void setAutoEnhance(bool enable)  { m_autoEnhance = enable; }

    /// 启用直方图均衡化（改善低对比度场景）
    void setHistogramEqualization(bool enable) { m_histEq = enable; }

    /// 启用高斯去噪（减少传感器噪声）
    void setDenoise(bool enable)      { m_denoise = enable; }

    /// 设置显示缩放（0 = 保持原始尺寸，如 640×480）
    void setDisplaySize(int width, int height) {
        m_displayWidth = width;
        m_displayHeight = height;
    }

    /// 设置断流重连间隔（毫秒），默认 3000ms
    void setReconnectInterval(int ms) { m_reconnectMs = ms; }

    /// 模型是否已加载
    bool isModelLoaded() const { return m_detector.isLoaded(); }

    /// 流是否已打开
    bool isStreamOpen() const { return m_streamOpened; }

public slots:
    /// 开始逐帧处理（由线程 started 信号触发）
    void start();

    /// 处理一帧（由 QTimer 驱动）
    void processFrame();

    /// 停止处理
    void stop();

signals:
    /// 处理完成的帧图像（已绘制检测框，RGB888 格式）
    void frameReady(const QImage &frame);

    /// 每帧的原始检测结果（可选：用于 QML 标签列表等）
    void detectionsReady(const std::vector<OnnxDetection> &detections);

    /// 流状态变化
    void streamStatusChanged(bool connected);

    /// 错误信息
    void errorOccurred(const QString &msg);

    /// 处理已停止
    void finished();

private:
    // ── 核心组件 ──
    OnnxYoloDetector m_detector;        // ONNX YOLOv3-tiny 检测器
    cv::VideoCapture m_capture;         // OpenCV 视频捕获

    // ── 流配置 ──
    QString m_streamUrl;                // 当前 RTSP/RTMP 地址
    bool    m_tcpTransport = true;      // 使用 TCP 传输
    bool    m_streamOpened = false;     // 流是否已成功打开
    int     m_reconnectMs = 3000;       // 断流重连间隔

    // ── 定时器 ──
    QTimer *m_timer = nullptr;

    // ── 状态 ──
    std::atomic<bool> m_running{false};
    int m_reconnectCount = 0;           // 重连尝试计数

    // ── 帧率控制 ──
    int m_targetFps = 25;

    // ── 绘制参数 ──
    bool m_drawBoxes = true;
    cv::Scalar m_boxColor = cv::Scalar(0, 255, 0);  // BGR 绿色

    // ── 图像处理参数 ──
    bool m_autoEnhance = false;         // 自动增强
    bool m_histEq     = false;          // 直方图均衡化
    bool m_denoise    = false;          // 高斯去噪
    int  m_displayWidth  = 0;           // 显示宽度（0=原始尺寸）
    int  m_displayHeight = 0;           // 显示高度

    // ── 内部方法 ──
    /// 对抓取的帧做基础图像处理
    void applyImageProcessing(cv::Mat &frame);

    /// 在帧上绘制检测框和标签
    void drawDetections(cv::Mat &frame,
                        const std::vector<OnnxDetection> &detections);

    /// 尝试重新连接流
    bool tryReconnect();
};

#endif // STREAMPROCESSOR_H
