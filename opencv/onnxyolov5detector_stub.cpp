#include "onnxyolov5detector.h"

// 未安装 MinGW 兼容的 ONNX Runtime 时使用的空实现。
// 视频显示和其他功能仍可运行，YOLO 推理保持禁用。
struct OnnxYoloV5DetectorImpl {};

OnnxYoloV5Detector::OnnxYoloV5Detector(QObject *parent)
    : QObject(parent)
{
}

OnnxYoloV5Detector::~OnnxYoloV5Detector() = default;

bool OnnxYoloV5Detector::loadModel(const std::string &)
{
    m_loaded = false;
    return false;
}

bool OnnxYoloV5Detector::loadClassNames(const std::string &)
{
    return false;
}

void OnnxYoloV5Detector::setClassNames(const std::vector<std::string> &names)
{
    m_classNames = names;
}

void OnnxYoloV5Detector::setConfThreshold(float thresh) { m_confThreshold = thresh; }
void OnnxYoloV5Detector::setNmsThreshold(float thresh)  { m_nmsThreshold = thresh; }
void OnnxYoloV5Detector::setNumThreads(int n)            { m_numThreads = n; }

void OnnxYoloV5Detector::setInputSize(int width, int height)
{
    m_inputWidth = width;
    m_inputHeight = height;
}

void OnnxYoloV5Detector::setNumClasses(int n)
{
    if (n > 0)
        m_numClasses = n;
}

bool OnnxYoloV5Detector::detect(const cv::Mat &, std::vector<OnnxDetection> &detections)
{
    detections.clear();
    return false;
}
