#include <windows.h>
#include <string>
#include "mqtt.h"

void PublishMQTT(const char* message) {
	std::string cmd = std::string("-h 54.180.119.169 -t rpi/motor -m \"") + message + "\"";
	ShellExecuteA(NULL, "open", "mosquitto_pub.exe", cmd.c_str(), NULL, SW_HIDE);
}