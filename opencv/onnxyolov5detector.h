// onnxyolov5detector.h — 基于 ONNX Runtime 的 YOLOv5n 目标检测器
//
// 与 OnnxYoloDetector（YOLOv3-tiny）接口兼容，可互换使用。
// 区别：
//   1. YOLOv5 单输出 [1, N, 5+K]（v3-tiny 是双输出需要手动锚点解码）
//   2. 坐标已在模型内部解码为像素空间，无需 sigmoid/exp/锚点计算
//   3. 后处理大幅简化
//
// 典型用法：
// @code
//   OnnxYoloV5Detector detector;
//   detector.loadModel("yolov5n.onnx");
//   detector.loadClassNames("coco.names");
//   detector.setConfThreshold(0.5f);
//
//   std::vector<OnnxDetection> detections;
//   detector.detect(frame, detections);
// @endcode
//
// YOLOv5n ONNX 导出命令（Ultralytics）：
//   python export.py --weights yolov5n.pt --include onnx --imgsz 416 --opset 12

#ifndef ONNXYOLOV5DETECTOR_H
#define ONNXYOLOV5DETECTOR_H

#include <QObject>
#include <opencv2/core.hpp>
#include <vector>
#include <string>
#include <memory>

// 复用同一个检测结果结构体
#include "onnxyolodetector.h"

// PIMPL 实现类（内部持有 ONNX Runtime 对象，定义在 .cpp 中）
struct OnnxYoloV5DetectorImpl;

class OnnxYoloV5Detector : public QObject
{
    Q_OBJECT
public:
    explicit OnnxYoloV5Detector(QObject *parent = nullptr);
    ~OnnxYoloV5Detector() override;

    // ── 模型加载 ──────────────────────────────────────────
    bool loadModel(const std::string &onnxPath);
    bool loadClassNames(const std::string &namesPath);
    void setClassNames(const std::vector<std::string> &names);

    // ── 参数调节 ──────────────────────────────────────────
    void setConfThreshold(float thresh);
    void setNmsThreshold(float thresh);
    void setInputSize(int width, int height);
    void setNumClasses(int n);
    void setNumThreads(int n);

    // ── 检测入口 ──────────────────────────────────────────
    bool detect(const cv::Mat &frame, std::vector<OnnxDetection> &detections);

    bool isLoaded() const { return m_loaded; }
    int classCount() const { return static_cast<int>(m_classNames.size()); }

signals:
    void errorOccurred(const QString &message);

private:
    std::unique_ptr<OnnxYoloV5DetectorImpl> m_impl;

    std::vector<std::string> m_classNames;

    float m_confThreshold = 0.5f;
    float m_nmsThreshold  = 0.4f;
    int   m_inputWidth    = 416;
    int   m_inputHeight   = 416;
    int   m_numClasses    = 80;
    int   m_numThreads    = 4;
    bool  m_loaded        = false;
    // Last class count inferred from the output shape (avoids spamming the
    // "Auto-detected" hint on every frame).
    int   m_lastAutoDetectedClasses = -1;

    // LetterBox 预处理参数（detect 时计算，供后处理坐标还原用）
    float m_letterBoxScale = 1.0f;
    int   m_letterBoxPadX  = 0;
    int   m_letterBoxPadY  = 0;
};

#endif // ONNXYOLOV5DETECTOR_H
