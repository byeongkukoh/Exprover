#pragma once

void AsyncPublish(const char* message);
void PublishMQTT(const char* message);
void SetMotorTopic(const std::string& topic);
void CleanupMQTT();