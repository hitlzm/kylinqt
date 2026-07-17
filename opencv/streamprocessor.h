// streamprocessor.h — 视频帧 ONNX YOLOv3-tiny 检测处理器
//
// 本身不拉流。从 VlcVideoItem 获取解码帧（grabFrame），
// 经 OpenCV + ONNX 检测 + 绘制后，回传 VlcVideoItem 显示
// （submitProcessedFrame），同时通过 frameReady 信号对外广播。
//
// 数据流：
//   VlcVideoItem::grabFrame()         — libvlc 解码的 RGBA QImage
//        ↓
//   StreamProcessor::processFrame()   — QTimer 驱动，工作线程
//        ↓  QImage(RGBA) → cv::Mat(BGR)
//        ↓  图像预处理 / ONNX 检测 / 绘制检测框
//        ↓  cv::Mat → QImage(RGB)
//        ↓
//   VlcVideoItem::submitProcessedFrame()  — 回传显示
//   emit frameReady(img)                  — 同时对外广播
//
// 典型用法：
// @code
//   // VlcVideoItem 在 QML 中创建，负责拉流
//   // StreamProcessor 在 C++ 中创建，负责检测
//   QThread *thread = new QThread;
//   StreamProcessor *proc = new StreamProcessor;
//   proc->setVideoSource(vlcItem);
//   proc->loadYoloModel("yolov3-tiny.onnx", "coco.names");
//   proc->moveToThread(thread);
//   QObject::connect(thread, &QThread::started, proc, &StreamProcessor::start);
//   QObject::connect(proc, &StreamProcessor::frameReady, ...);
//   thread->start();
// @endcode

#ifndef STREAMPROCESSOR_H
#define STREAMPROCESSOR_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <atomic>
#include "onnxyolodetector.h"

class VlcVideoItem;  // 前向声明

/**
 * @brief 视频帧处理工作线程（ONNX 检测 + 回传 VlcVideoItem 显示）
 *
 * 运行在独立 QThread 中。不持有任何拉流/解码资源，
 * 所有帧均来自外部 VlcVideoItem。OpenCV 仅用于：
 *  1. QImage(RGBA) → cv::Mat(BGR) 格式转换
 *  2. 可选的基础图像处理（去噪、增强等）
 *  3. YOLOv3-tiny ONNX 推理
 *  4. 绘制检测框和标签
 *  5. cv::Mat → QImage(RGB) 转回
 */
class StreamProcessor : public QObject
{
    Q_OBJECT
public:
    explicit StreamProcessor(QObject *parent = nullptr);
    ~StreamProcessor() override;

    // ── 视频源 ────────────────────────────────────────────
    /// 设置视频来源（VlcVideoItem，负责 libvlc 拉流 + 最终显示）
    void setVideoSource(VlcVideoItem *source);

    // ── 模型加载 ──────────────────────────────────────────
    bool loadYoloModel(const QString &onnxPath,
                       const QString &namesPath = QString());

    // ── 参数调节 ──────────────────────────────────────────
    void setTargetFps(int fps);
    void setConfThreshold(float t);
    void setNmsThreshold(float t);
    void setInputSize(int width, int height);

    void setDrawBoxes(bool draw) { m_drawBoxes = draw; }
    void setBoxColor(int b, int g, int r) {
        m_boxColor = cv::Scalar(b, g, r);
    }

    // ── 基础图像处理开关 ─────────────────────────────────
    void setAutoEnhance(bool enable)  { m_autoEnhance = enable; }
    void setHistogramEqualization(bool enable) { m_histEq = enable; }
    void setDenoise(bool enable)      { m_denoise = enable; }
    void setDisplaySize(int width, int height) {
        m_displayWidth = width;
        m_displayHeight = height;
    }

    // ── 状态查询 ──────────────────────────────────────────
    bool isModelLoaded() const { return m_detector.isLoaded(); }
    bool hasVideoSource() const { return m_videoSource != nullptr; }

    /// 最新检测框中置信度最高的目标中心像素坐标（-1 表示无检测结果）
    int centerX() const { return m_centerX; }
    int centerY() const { return m_centerY; }

public slots:
    //最后把StreamProcessor放入新线程运行
    void start();
    void processFrame();
    void stop();

signals:
    void frameReady(const QImage &frame);
    void detectionsReady(const std::vector<OnnxDetection> &detections);
    void errorOccurred(const QString &msg);
    void finished();

private:
    void applyImageProcessing(cv::Mat &frame);
    void drawDetections(cv::Mat &frame,
                        const std::vector<OnnxDetection> &detections);

    OnnxYoloDetector m_detector;
    VlcVideoItem *m_videoSource = nullptr;   // 外部 VlcVideoItem（拉流+显示）

    QTimer *m_timer = nullptr;
    std::atomic<bool> m_running{false};

    int m_targetFps = 30;

    // 绘制参数
    bool m_drawBoxes = true;
    cv::Scalar m_boxColor = cv::Scalar(0, 255, 0);

    // 图像处理参数
    bool m_autoEnhance = false;
    bool m_histEq     = false;
    bool m_denoise    = false;
    int  m_displayWidth  = 0;
    int  m_displayHeight = 0;

    // 检测框中心点（最高置信度目标）
    //外引导模式时可根据CCD视场角得到转台的方位角与俯仰角应转动的角度
    int m_centerX = -1;
    int m_centerY = -1;
};

#endif // STREAMPROCESSOR_H
