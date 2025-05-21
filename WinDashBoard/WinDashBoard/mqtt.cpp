#include <iostream>
#include <string>
#include <functional>
#include <mutex>
#include <thread>

#include "mqtt/async_client.h"
#include "mqtt.h"

const std::string SERVER_ADDRESS = "tcp://54.180.119.169";
const std::string CLIENT_ID = "WinClient";
const std::string TOPIC = "rpi/motor";

void PublishMQTT(const char* message) {
	static mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
	
	try {
		if (!client.is_connected()) {
			client.connect()->wait();
		}

		mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, message);
		pubmsg->set_qos(1);
		client.publish(pubmsg)->wait();
	}
	catch (const mqtt::exception& exc) {
		std::cerr << "[MQTT ERROR] " << exc.what() << std::endl;
	}
}

void AsyncPublish(const char* message) {
	std::thread([message]() {
		PublishMQTT(message);
		}).detach();
}