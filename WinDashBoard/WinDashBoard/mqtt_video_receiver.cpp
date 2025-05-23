#include <iostream>
#include <thread>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <string>

#include "base64.h"
#include "command.h"
#include "mqtt/async_client.h"
#include "mqtt_video_receiver.h"

extern HWND hPictureBox;
extern HBITMAP hCurrentBitmap;

namespace mqtt {
	const std::string message::EMPTY_STR;
}

const std::string SERVER_ADDRESS = "tcp://54.180.119.169";
const std::string CLIENT_ID = "WinVideoReceiverClient";
const std::string VIDEO_TOPIC = "rpi/video";

// Callback 함수: 메시지 수신 시 호출
void OnVideoMessage(const std::string& base64) {
	std::cout << "[MQTT] 영상 메시지 수신: " << base64.length() << " 바이트" << std::endl;
	
	std::vector<uchar> jpg_bytes = base64_decode(base64);
	cv::Mat image = cv::imdecode(jpg_bytes, cv::IMREAD_COLOR);

	if (image.empty()) {
		AddLogMsg(L"ERROR | 디코딩  실패 (빈 이미지)");
		//std::cerr << "[ERROR] 디코딩 실패 (빈 이미지)" << std::endl;
		return;
	}

	// 180도 회전 (카메라가 거꾸로 달린 경우)
	cv::rotate(image, image, cv::ROTATE_180);

	// 320x240 → 500x375 (4:3 비율 유지)
	cv::Mat resized;
	cv::resize(image, resized, cv::Size(500, 375), 0, 0, cv::INTER_LINEAR);

	HBITMAP hNewBitmap = MatToHBITMAP(resized);
	if (hNewBitmap) {
		// 이전 비트맵 제거
		if (hCurrentBitmap) {
			DeleteObject(hCurrentBitmap);
		}
		hCurrentBitmap = hNewBitmap;

		// GUI에 표시
		SendMessage(hPictureBox, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hNewBitmap);
	}

	std::cout << "[MQTT] 이미지 디코딩 성공: " << image.cols << "x" << image.rows << std::endl;

	// TODO: WinAPI로 화면에 출력
}

void StartVideoReceiver() {
	std::thread([] {
		mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

		// 메시지 수신 시 동작할 콜백 등록
		class callback : public virtual mqtt::callback {
		public:
			void message_arrived(mqtt::const_message_ptr msg) override {
				OnVideoMessage(msg->to_string());
			}
		};

		callback cb;
		client.set_callback(cb);

		try {
			client.connect()->wait();
			client.subscribe(VIDEO_TOPIC, 1)->wait();

			AddLogMsg(L"MQTT | 영상 수신 시작");
			//std::cout << "[MQTT] 영상 수신 시작" << std::endl;

			// 스레드 유지
			while (true) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		catch (const mqtt::exception& err) {
			std::string errorMsg = std::string("MQTT | ERROR | ") + err.what();
			std::wstring wMsg = ConvertToWString(errorMsg);

			AddLogMsg(wMsg.c_str());
			//std::cerr << "[MQTT ERROR] " << err.what() << std::endl;
		}
	}).detach();
}

HBITMAP MatToHBITMAP(const cv::Mat& mat) {
	if (mat.empty()) return nullptr;

	cv::Mat bgr;
	if (mat.channels() == 1)
		cv::cvtColor(mat, bgr, cv::COLOR_GRAY2BGR);
	else if (mat.channels() == 3)
		bgr = mat;
	else
		return nullptr;

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = bgr.cols;
	bmi.bmiHeader.biHeight = -bgr.rows;  // top-down
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	void* bits = nullptr;
	HDC hdc = GetDC(nullptr);
	HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
	ReleaseDC(nullptr, hdc);

	if (bits) {
		memcpy(bits, bgr.data, bgr.cols * bgr.rows * 3);
	}
	return hBitmap;
}