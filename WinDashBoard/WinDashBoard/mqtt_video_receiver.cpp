#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>

#include "base64.h"
#include "mqtt/async_client.h"
#include "mqtt_video_receiver.h"

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
		std::cerr << "[ERROR] 디코딩 실패 (빈 이미지)" << std::endl;
		return;
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

			std::cout << "[MQTT] 영상 수신 시작" << std::endl;

			// 스레드 유지
			while (true) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		catch (const mqtt::exception& err) {
			std::cerr << "[MQTT ERROR] " << err.what() << std::endl;
		}
	}).detach();
}
