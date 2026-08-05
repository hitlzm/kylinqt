// onnxyolov5detector.cpp — ONNX Runtime YOLOv5n 检测器实现
//
// ══════════════════════════════════════════════════════════════════════════════
//  YOLOv5n ONNX 推理数据流（与 YOLOv3-tiny 的关键区别见文末对比表）
// ══════════════════════════════════════════════════════════════════════════════
//
//  步骤① 预处理 LetterBox + NCHW blob（同 YOLOv3-tiny，复用 preprocessFrame）
//  步骤② ONNX 推理 session->Run()
//          输入:  [1, 3, 416, 416]
//          输出:  [1, N, 5+K]  单张量，N = 10647（416×416 时）
//                 N = (52² + 26² + 13²) × 3
//  步骤③ 后处理
//          YOLOv5 已将 Detect 层打包进 ONNX 模型（sigmoid + 坐标解码），
//          输出每行 = [cx, cy, w, h, obj, cls_0..cls_{K-1}]
//          - cx/cy/w/h 在输入图像像素空间，无需手动 sigmoid/exp/锚点解码
//          - obj / cls 已过 sigmoid，区间 [0,1]
//          → 直接提取 + LetterBox 坐标还原 + NMS
//
// ══════════════════════════════════════════════════════════════════════════════
//  vs YOLOv3-tiny 差异
// ══════════════════════════════════════════════════════════════════════════════
//  维度          | YOLOv3-tiny           | YOLOv5n
//  ─────────────┼───────────────────────┼───────────────────────
//  输出数量      | 2 个张量（2 尺度）     | 1 个张量（3 尺度拼接）
//  输出形状      | [1,255,13,13] 等      | [1, N, 5+K]
//  坐标编码      | tx,ty,tw,th 原始值    | cx,cy,w,h 已解码（像素空间）
//  需要 sigmoid  | ✓ 手动                | ✗ 模型已做
//  需要 exp      | ✓ 手动                | ✗ 模型已做
//  需要锚点解码  | ✓ 手动 kAnchors       | ✗ 模型已做
//  网格偏移      | ✓ 手动 gx,gy          | ✗ 模型已做
//  LetterBox还原 | ✓                     | ✓ 同样需要
//  NMS           | ✓ cv::dnn::NMSBoxes   | ✓ 同样

#include "onnxyolov5detector.h"
#include <onnxruntime_cxx_api.h>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <iostream>
#include <cmath>
#include <QDebug>

// ══════════════════════════════════════════════════════════════════════════════
// PIMPL 实现 — 封装所有 ONNX Runtime C++ 对象
// ══════════════════════════════════════════════════════════════════════════════
struct OnnxYoloV5DetectorImpl {
    std::unique_ptr<Ort::Env> env;
    std::unique_ptr<Ort::SessionOptions> sessionOptions;
    std::unique_ptr<Ort::Session> session;

    std::string inputName;
    std::vector<std::string> outputNames;

    std::vector<int64_t> inputShape;

    void release() {
        session.reset();
        sessionOptions.reset();
        env.reset();
        inputName.clear();
        outputNames.clear();
        inputShape.clear();
    }
};

// ══════════════════════════════════════════════════════════════════════════════
// 构造 / 析构
// ══════════════════════════════════════════════════════════════════════════════

OnnxYoloV5Detector::OnnxYoloV5Detector(QObject *parent)
    : QObject(parent)
    , m_impl(std::make_unique<OnnxYoloV5DetectorImpl>())
{
}

OnnxYoloV5Detector::~OnnxYoloV5Detector() {
    m_impl->release();
}

// ══════════════════════════════════════════════════════════════════════════════
// 参数设置
// ══════════════════════════════════════════════════════════════════════════════

void OnnxYoloV5Detector::setConfThreshold(float thresh) { m_confThreshold = thresh; }
void OnnxYoloV5Detector::setNmsThreshold(float thresh)  { m_nmsThreshold  = thresh; }
void OnnxYoloV5Detector::setNumThreads(int n)            { m_numThreads    = n; }

void OnnxYoloV5Detector::setInputSize(int width, int height) {
    m_inputWidth  = width;
    m_inputHeight = height;
}

void OnnxYoloV5Detector::setNumClasses(int n) {
    if (n > 0) m_numClasses = n;
}

// ══════════════════════════════════════════════════════════════════════════════
// 模型加载（与 YOLOv3-tiny 一致）
// ══════════════════════════════════════════════════════════════════════════════

bool OnnxYoloV5Detector::loadModel(const std::string &onnxPath) {
    m_impl->release();
    m_loaded = false;

    try {
        m_impl->env = std::make_unique<Ort::Env>(
            ORT_LOGGING_LEVEL_WARNING, "OnnxYoloV5Detector");

        m_impl->sessionOptions = std::make_unique<Ort::SessionOptions>();
        m_impl->sessionOptions->SetIntraOpNumThreads(m_numThreads);
        m_impl->sessionOptions->SetGraphOptimizationLevel(
            GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

        m_impl->session = std::make_unique<Ort::Session>(
            *m_impl->env, onnxPath.c_str(), *m_impl->sessionOptions);

        Ort::AllocatorWithDefaultOptions allocator;

        // ── 输入信息 ──
        {
            size_t numInputs = m_impl->session->GetInputCount();
            if (numInputs != 1) {
                emit errorOccurred(QString("YOLOv5n: expected 1 input, got %1").arg(numInputs));
                m_impl->release();
                return false;
            }

            auto namePtr = m_impl->session->GetInputNameAllocated(0, allocator);
            m_impl->inputName = namePtr.get();

            Ort::TypeInfo typeInfo = m_impl->session->GetInputTypeInfo(0);
            auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
            m_impl->inputShape = tensorInfo.GetShape();

            if (m_impl->inputShape.size() == 4) {
                if (m_impl->inputShape[0] == -1) m_impl->inputShape[0] = 1;
                if (m_impl->inputShape[2] == -1) m_impl->inputShape[2] = m_inputHeight;
                if (m_impl->inputShape[3] == -1) m_impl->inputShape[3] = m_inputWidth;
            }

            qDebug() << "[OnnxYoloV5] Input:" << QString::fromStdString(m_impl->inputName)
                     << "shape: [" << m_impl->inputShape[0]
                     << m_impl->inputShape[1]
                     << m_impl->inputShape[2]
                     << m_impl->inputShape[3] << "]";
        }

        // ── 输出信息 ──
        {
            size_t numOutputs = m_impl->session->GetOutputCount();
            if (numOutputs < 1) {
                emit errorOccurred("YOLOv5n: model has no outputs");
                m_impl->release();
                return false;
            }

            m_impl->outputNames.resize(numOutputs);
            for (size_t i = 0; i < numOutputs; ++i) {
                auto namePtr = m_impl->session->GetOutputNameAllocated(i, allocator);
                m_impl->outputNames[i] = namePtr.get();

                Ort::TypeInfo typeInfo = m_impl->session->GetOutputTypeInfo(i);
                auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
                auto shape = tensorInfo.GetShape();

                qDebug() << "[OnnxYoloV5] Output" << i
                         << ":" << QString::fromStdString(m_impl->outputNames[i])
                         << "shape:" << (shape.size() >= 3 ?
                            QString("[%1, %2, %3]").arg(shape[0]).arg(shape[1]).arg(shape[2]) :
                            "?");
            }
        }

        m_loaded = true;
        qDebug() << "[OnnxYoloV5] Model loaded:" << QString::fromStdString(onnxPath);
        return true;

    } catch (const Ort::Exception &e) {
        emit errorOccurred(QString("ONNX Runtime error: %1").arg(e.what()));
        m_impl->release();
        return false;
    } catch (const std::exception &e) {
        emit errorOccurred(QString("Failed to load model: %1").arg(e.what()));
        m_impl->release();
        return false;
    }
}

bool OnnxYoloV5Detector::loadClassNames(const std::string &namesPath) {
    std::ifstream file(namesPath);
    if (!file.is_open()) {
        emit errorOccurred(QString("Cannot open class names file: %1")
                          .arg(QString::fromStdString(namesPath)));
        return false;
    }

    m_classNames.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        m_classNames.push_back(line);
    }
    file.close();

    qDebug() << "[OnnxYoloV5] Loaded" << m_classNames.size() << "class names";
    return !m_classNames.empty();
}

void OnnxYoloV5Detector::setClassNames(const std::vector<std::string> &names) {
    m_classNames = names;
}

// ══════════════════════════════════════════════════════════════════════════════
// 预处理 — LetterBox + NCHW blob（与 YOLOv3-tiny 完全相同）
// ══════════════════════════════════════════════════════════════════════════════

static cv::Mat preprocessV5(const cv::Mat &frame, int w, int h,
                             float &outScale, int &outPadX, int &outPadY) {
    const float scale = std::min(static_cast<float>(w) / frame.cols,
                                 static_cast<float>(h) / frame.rows);
    const int newW = static_cast<int>(std::round(frame.cols * scale));
    const int newH = static_cast<int>(std::round(frame.rows * scale));

    outScale = scale;
    outPadX  = (w - newW) / 2;
    outPadY  = (h - newH) / 2;

    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(newW, newH));

    cv::Mat canvas(h, w, CV_8UC3, cv::Scalar(114, 114, 114));
    resized.copyTo(canvas(cv::Rect(outPadX, outPadY, newW, newH)));

    // HWC → CHW，uint8 → float，BGR → RGB，归一化 [0,1]，一次遍历完成
    int sz[] = {1, 3, h, w};
    cv::Mat blob(4, sz, CV_32F);

    for (int y = 0; y < h; ++y) {
        const uint8_t *srcRow = canvas.ptr<uint8_t>(y);
        for (int x = 0; x < w; ++x) {
            const uint8_t *px = srcRow + x * 3;
            blob.ptr<float>(0, 0)[y * w + x] = px[2] / 255.0f;  // R
            blob.ptr<float>(0, 1)[y * w + x] = px[1] / 255.0f;  // G
            blob.ptr<float>(0, 2)[y * w + x] = px[0] / 255.0f;  // B
        }
    }

    return blob;
}

// ══════════════════════════════════════════════════════════════════════════════
// 后处理 — YOLOv5 单输出张量解析
//
// YOLOv5 ONNX 输出格式（含 Detect 层，默认导出）：
//   [1, N, 5+K]
//   每行 = [cx, cy, w, h, obj_conf, cls_0, ..., cls_{K-1}]
//
//   cx, cy : 目标中心在输入图像上的像素坐标（0 ~ inputW/H）
//   w, h   : 边界框宽高（像素）
//   obj    : 目标置信度，已 sigmoid，[0, 1]
//   cls_*  : 各类别分数，已 sigmoid，[0, 1]
//
// 坐标还原（LetterBox 逆向）：
//   orig_x = (cx - w/2 - padX) / scale
//   orig_y = (cy - h/2 - padY) / scale
//   orig_w = w / scale
//   orig_h = h / scale
// ══════════════════════════════════════════════════════════════════════════════

static void postProcessYoloV5(
    const float *data,          // [1, N, 5+K]
    int numDetections,          // N
    int numClasses,             // K
    int inputW, int inputH,     // 网络输入尺寸（416, 416）
    int frameW, int frameH,     // 原始帧尺寸
    float confThreshold,
    float nmsThreshold,
    float letterBoxScale,
    int letterBoxPadX,
    int letterBoxPadY,
    std::vector<int> &classIds,
    std::vector<float> &confidences,
    std::vector<cv::Rect> &boxes)
{
    const int rowSize = 5 + numClasses;  // 每行的属性数

    for (int i = 0; i < numDetections; ++i) {
        const float *row = data + i * rowSize;

        const float cx = row[0];          // 已解码，像素空间
        const float cy = row[1];
        const float bw = row[2];
        const float bh = row[3];
        const float objConf = row[4];     // 已 sigmoid

        // ── 快速跳过极低目标置信度 ──
        if (objConf < confThreshold * 0.1f)
            continue;

        // ── 找最佳类别 ──
        float maxClsScore = 0.0f;
        int bestClass = -1;
        for (int c = 0; c < numClasses; ++c) {
            const float score = row[5 + c];
            if (score > maxClsScore) {
                maxClsScore = score;
                bestClass = c;
            }
        }

        const float confidence = objConf * maxClsScore;
        if (confidence < confThreshold)
            continue;

        // ── 从 LetterBox 输入画布坐标还原为原始图像坐标 ──
        const float left   = cx - bw * 0.5f;
        const float top    = cy - bh * 0.5f;
        const float width  = bw;
        const float height = bh;

        const int x1 = static_cast<int>((left   - static_cast<float>(letterBoxPadX)) / letterBoxScale);
        const int y1 = static_cast<int>((top    - static_cast<float>(letterBoxPadY)) / letterBoxScale);
        const int x2 = static_cast<int>((left + width - static_cast<float>(letterBoxPadX)) / letterBoxScale);
        const int y2 = static_cast<int>((top + height - static_cast<float>(letterBoxPadY)) / letterBoxScale);

        // ── 裁剪到图像范围内 ──
        const int cx1 = std::max(0, x1);
        const int cy1 = std::max(0, y1);
        const int cx2 = std::min(frameW, x2);
        const int cy2 = std::min(frameH, y2);

        if (cx2 <= cx1 || cy2 <= cy1)
            continue;

        classIds.push_back(bestClass);
        confidences.push_back(confidence);
        boxes.push_back(cv::Rect(cx1, cy1, cx2 - cx1, cy2 - cy1));
    }

    // ── NMS 去重 ──
    if (!boxes.empty()) {
        std::vector<int> nmsIndices;
        cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, nmsIndices);

        std::vector<int> filteredIds;
        std::vector<float> filteredConfs;
        std::vector<cv::Rect> filteredBoxes;
        filteredIds.reserve(nmsIndices.size());
        filteredConfs.reserve(nmsIndices.size());
        filteredBoxes.reserve(nmsIndices.size());

        for (int idx : nmsIndices) {
            filteredIds.push_back(classIds[idx]);
            filteredConfs.push_back(confidences[idx]);
            filteredBoxes.push_back(boxes[idx]);
        }

        classIds.swap(filteredIds);
        confidences.swap(filteredConfs);
        boxes.swap(filteredBoxes);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// 检测入口 — 预处理 → 推理 → 后处理
// ══════════════════════════════════════════════════════════════════════════════

bool OnnxYoloV5Detector::detect(const cv::Mat &frame,
                                 std::vector<OnnxDetection> &detections) {
    detections.clear();

    if (!m_loaded || !m_impl->session) {
        std::cerr << "[OnnxYoloV5] Model not loaded." << std::endl;
        return false;
    }
    if (frame.empty()) {
        std::cerr << "[OnnxYoloV5] Input frame is empty." << std::endl;
        return false;
    }

    try {
        // ══ 步骤① 预处理 LetterBox + NCHW ══
        float letterBoxScale;
        int letterBoxPadX, letterBoxPadY;
        // 输入尺寸以模型实际 shape 为准（loadModel 时解析，动态维度回退到 setInputSize）
        std::vector<int64_t> inputShape = m_impl->inputShape;
        if (inputShape.size() != 4) {
            inputShape = {1, 3, m_inputHeight, m_inputWidth};
        }
        if (inputShape[0] <= 0) inputShape[0] = 1;
        if (inputShape[1] <= 0) inputShape[1] = 3;
        if (inputShape[2] <= 0) inputShape[2] = m_inputHeight;
        if (inputShape[3] <= 0) inputShape[3] = m_inputWidth;
        const int inputW = static_cast<int>(inputShape[3]);
        const int inputH = static_cast<int>(inputShape[2]);

        cv::Mat blob = preprocessV5(frame, inputW, inputH,
                                     letterBoxScale, letterBoxPadX, letterBoxPadY);
        m_letterBoxScale = letterBoxScale;
        m_letterBoxPadX  = letterBoxPadX;
        m_letterBoxPadY  = letterBoxPadY;

        // ══ 步骤② 构造输入张量 ══
        Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(
            OrtArenaAllocator, OrtMemTypeDefault);

        size_t inputElementCount = static_cast<size_t>(
            inputShape[0] * inputShape[1] * inputShape[2] * inputShape[3]);

        Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
            memoryInfo,
            reinterpret_cast<float *>(blob.data),
            inputElementCount,
            inputShape.data(),
            inputShape.size()
        );

        // ══ 步骤③ ONNX 推理 ══
        std::vector<const char *> inputNames  = {m_impl->inputName.c_str()};
        std::vector<const char *> outputNames;
        outputNames.reserve(m_impl->outputNames.size());
        for (auto &name : m_impl->outputNames)
            outputNames.push_back(name.c_str());

        auto outputTensors = m_impl->session->Run(
            Ort::RunOptions{nullptr},
            inputNames.data(), &inputTensor, 1,
            outputNames.data(), outputNames.size()
        );

        // ══ 步骤④ 后处理：单输出张量，直接解析 ══
        // YOLOv5n 导出时 Detect 层已打包进模型
        // 输出: [1, N, 5+K]  每行已是解码后的像素坐标 + sigmoid 置信度
        auto &tensor = outputTensors[0];
        float *outputData = tensor.GetTensorMutableData<float>();
        auto shapeInfo = tensor.GetTensorTypeAndShapeInfo();
        auto shape = shapeInfo.GetShape();

        int numDetections = 0;
        int numAttrs = 0;
        if (shape.size() >= 3) {
            numDetections = static_cast<int>(shape[1]);  // N
            numAttrs      = static_cast<int>(shape[2]);  // 5+K
        } else if (shape.size() == 2) {
            numDetections = static_cast<int>(shape[0]);
            numAttrs      = static_cast<int>(shape[1]);
        }

        // 从输出张量推断类别数（若用户未通过 setNumClasses 设置）
        const int actualNumClasses = numAttrs - 5;
        const int K = (m_numClasses > 0 && m_numClasses < actualNumClasses)
                          ? m_numClasses
                          : actualNumClasses;

        // 如果模型输出的类别数跟预期不一致，更新内部值
        if (K != m_numClasses) {
            qDebug() << "[OnnxYoloV5] Auto-detected" << K << "classes (from output shape)";
        }

        if (numDetections <= 0 || K <= 0) {
            std::cerr << "[OnnxYoloV5] Invalid output shape: ["
                      << shape.size() << " dims]" << std::endl;
            return false;
        }

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        postProcessYoloV5(
            outputData,
            numDetections,
            K,
            inputW, inputH,
            frame.cols, frame.rows,
            m_confThreshold,
            m_nmsThreshold,
            m_letterBoxScale,
            m_letterBoxPadX,
            m_letterBoxPadY,
            classIds,
            confidences,
            boxes
        );

        // ══ 步骤⑤ 组装最终结果 ══
        detections.reserve(boxes.size());
        for (size_t i = 0; i < boxes.size(); ++i) {
            OnnxDetection det;
            det.classId    = classIds[i];
            det.confidence = confidences[i];
            det.bbox       = boxes[i];

            if (det.classId >= 0 && det.classId < static_cast<int>(m_classNames.size()))
                det.className = m_classNames[det.classId];
            else
                det.className = "cls_" + std::to_string(det.classId);

            detections.push_back(det);
        }

        return !detections.empty();

    } catch (const Ort::Exception &e) {
        std::cerr << "[OnnxYoloV5] Inference error: " << e.what() << std::endl;
        return false;
    } catch (const cv::Exception &e) {
        std::cerr << "[OnnxYoloV5] OpenCV error: " << e.what() << std::endl;
        return false;
    }
}
