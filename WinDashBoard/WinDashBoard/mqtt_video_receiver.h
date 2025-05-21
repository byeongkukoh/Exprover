#pragma once
#include <opencv2/opencv.hpp>

void OnVideoMessage(const std::string& base64);
void StartVideoReceiver();
HBITMAP MatToHBITMAP(const cv::Mat& mat);