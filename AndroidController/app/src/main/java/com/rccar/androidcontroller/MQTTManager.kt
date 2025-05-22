package com.rccar.androidcontroller

import android.content.Context
import org.eclipse.paho.client.mqttv3.*
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence

class MqttManager {
    private lateinit var client: MqttClient
    private val serverIp = "tcp://54.180.119.169" // 포트 명시 (기본 MQTT 포트: 1883)
    private val clientId = "ANDROIDCONTROLLER"
    private val topic = "rpi/motor"

    fun connect() {
        try {
            // MemoryPersistence를 사용하여 클라이언트 상태를 메모리에 저장
            client = MqttClient(serverIp, clientId, MemoryPersistence())
            val options = MqttConnectOptions()
            options.isCleanSession = true

            client.connect(options)
            println("[MQTT] 연결 성공")
        } catch (e: Exception) {
            println("[MQTT] 연결 실패: ${e.message}")
            e.printStackTrace()
        }
    }

    fun publish(message: String) {
        if (::client.isInitialized && client.isConnected) {
            try {
                val mqttMessage = MqttMessage(message.toByteArray())
                client.publish(topic, mqttMessage)
                println("[MQTT] 메시지 발행: $message")
            } catch (e: Exception) {
                println("[MQTT] 발행 실패: ${e.message}")
                e.printStackTrace()
            }
        } else {
            println("[MQTT] 연결되지 않음. 발행 실패")
        }
    }

    // 연결 해제 메서드 (필요 시 호출)
    fun disconnect() {
        if (::client.isInitialized && client.isConnected) {
            try {
                client.disconnect()
                println("[MQTT] 연결 해제 성공")
            } catch (e: Exception) {
                println("[MQTT] 연결 해제 실패: ${e.message}")
                e.printStackTrace()
            }
        }
    }
}