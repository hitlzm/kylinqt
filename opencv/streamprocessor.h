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
#include <QElapsedTimer>
#include <atomic>
#include "onnxyolodetector.h"
#include "pixelkalmantracker.h"

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

    // ── 图像分辨率与视场角 ────────────────────────────────
    // void setImageResolution(int width, int height) {
    //     m_imageWidth  = width;
    //     m_imageHeight = height;
    // }
    // void setFov(float hFov, float vFov) {
    //     m_hFov = hFov;
    //     m_vFov = vFov;
    // }
    // int imageWidth()  const { return m_imageWidth; }
    // int imageHeight() const { return m_imageHeight; }
    // float hFov() const { return m_hFov; }
    // float vFov() const { return m_vFov; }

public slots:
    //最后把StreamProcessor放入新线程运行
    void start();
    void processFrame();
    void stop();
    // 这里槽函数监听外引导源切换为CCD相机时，以固定时间间隔发送角度信息给转台线程
    void CCDguide(){
        //如果为外引导模式，且外引导源为CCD相机

        // 目标无效（未检测到或长时间丢失），不发送角度指令
        if (m_centerX < 0 || m_centerY < 0)
            return;

        //计算对应俯仰角与框架角
        float Pitchangle = m_centerX / static_cast<float>(m_imageWidth) * m_hFov;
        float Yawangle   = m_centerY / static_cast<float>(m_imageHeight) * m_vFov;
        //按一定时间间隔发送

    };

    //监听图像分辨率与焦距变化，CCD设置发生变化时给出信号
    void imgSetChange(){
        //在这里改变图像分辨率与视场角
        
    };

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

    // 检测框中心点（最高置信度目标，经卡尔曼滤波后输出）
    // 外引导模式时可根据CCD视场角得到转台的方位角与俯仰角应转动的角度，广角模式下为55.27，32.26 远焦模式下为2.66，1.51
    int m_centerX = -1;
    int m_centerY = -1;

    // ── 像素坐标卡尔曼跟踪器 ──
    PixelKalmanTracker m_tracker;
    QElapsedTimer m_kalmanTimer;
    bool m_kalmanFirstFrame = true;

    // 图像分辨率与视场角（视场角随广角/远焦切换更新）
    int   m_imageWidth  = 1920;
    int   m_imageHeight = 1080;
    float m_hFov = 55.27f;   // 横向视场角（默认广角）
    float m_vFov = 32.26f;   // 纵向视场角（默认广角）
};

#endif // STREAMPROCESSOR_H
