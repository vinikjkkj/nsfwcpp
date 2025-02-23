#ifndef NSFW_H
#define NSFW_H

#include <array>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>

inline const char* input_name = "input_1";
inline const wchar_t* model_path = L"model.onnx";
inline const std::vector<std::string> labels = {"Drawing", "Hentai", "Neutral", "Porn", "Sexy"};

std::unordered_map<std::string, float> nsfw(std::vector<uchar> buff) {
    std::vector<uchar> image_data(buff.begin(), buff.end());
    cv::Mat in = cv::imdecode(image_data, cv::IMREAD_COLOR);
    if (in.empty()) {
        throw std::runtime_error("Failed to decode image");
    }

    cv::Mat resized;
    cv::resize(in, resized, cv::Size(224, 224));
    cv::Mat rgb;
    cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);
    rgb.convertTo(rgb, CV_32F, 1.0 / 255);

    std::array<int64_t, 4> input_dims = {1, 224, 224, 3};
    std::vector<float> input_tensor_values(1 * 224 * 224 * 3);

    for (int i = 0; i < 224; ++i) {
        for (int j = 0; j < 224; ++j) {
            for (int c = 0; c < 3; ++c) {
                input_tensor_values[i * (224 * 3) + j * 3 + c] = rgb.at<cv::Vec3f>(i, j)[c];
            }
        }
    }
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "nsfw");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    Ort::Session session(env, model_path, session_options);

    Ort::AllocatorWithDefaultOptions allocator;
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info, input_tensor_values.data(), input_tensor_values.size(),
        input_dims.data(), input_dims.size()
    );

    const char* output_name = session.GetOutputNameAllocated(0, allocator).get();

    auto output_tensors = session.Run(
        Ort::RunOptions{nullptr},
        &input_name, &input_tensor, 1,
        &output_name, 1
    );

    float* output_data = output_tensors.front().GetTensorMutableData<float>();

    std::unordered_map<std::string, float> result;
    for (size_t i = 0; i < labels.size(); ++i) {
        result[labels[i]] = output_data[i];
    }

    return result;
}

#endif