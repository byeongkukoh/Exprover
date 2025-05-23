#include <iostream>
#include <string>
#include <functional>
#include <mutex>
#include <thread>

#include "mqtt/async_client.h"
#include "mqtt.h"
#include "command.h"

const std::string SERVER_ADDRESS = "tcp://54.180.119.169";
const std::string CLIENT_ID = "WinClient";

std::string TOPIC;
static mqtt::async_client* client = nullptr;

void SetMotorTopic(const std::string& topic) {
    TOPIC = topic;
    std::wstring log = L"MQTT | Motor TOPIC set to: " + ConvertToWString(TOPIC);
    AddLogMsg(log.c_str());
}

void PublishMQTT(const char* message) {
    if (TOPIC.empty()) {
        AddLogMsg(L"MQTT | ERROR | TOPIC이 설정되지 않았습니다. 연결 먼저 하세요.");
        return;
    }

    std::wstring log = L"DEBUG | PublishMQTT called for topic: " + ConvertToWString(TOPIC) + L", message: " + ConvertToWString(message);
    AddLogMsg(log.c_str());

    if (!client) {
        client = new mqtt::async_client(SERVER_ADDRESS, CLIENT_ID);
        AddLogMsg(L"MQTT | Client initialized");
    }

    try {
        if (!client->is_connected()) {
            AddLogMsg(L"MQTT | Connecting to broker...");
            client->connect()->wait();
            AddLogMsg(L"MQTT | Connected to broker");
        }

        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, message);
        pubmsg->set_qos(1);
        client->publish(pubmsg)->wait();
        AddLogMsg((L"MQTT | Message published: " + ConvertToWString(message)).c_str());
    }
    catch (const mqtt::exception& exc) {
        std::string errorMsg = std::string("MQTT | ERROR | ") + exc.what();
        AddLogMsg(ConvertToWString(errorMsg).c_str());
    }
}

void AsyncPublish(const char* message) {
    std::wstring log = L"DEBUG | AsyncPublish called with message: " + ConvertToWString(message);
    AddLogMsg(log.c_str());
    std::thread([message]() {
        PublishMQTT(message);
        }).detach();
}

void CleanupMQTT() {
    if (client) {
        if (client->is_connected()) {
            client->disconnect()->wait();
        }
        delete client;
        client = nullptr;
    }
}