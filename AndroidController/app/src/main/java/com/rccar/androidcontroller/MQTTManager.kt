package com.rccar.androidcontroller

import org.eclipse.paho.client.mqttv3.*
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence

import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

fun currentTimeString(): String {
    val sdf = SimpleDateFormat("MM-dd HH:mm:ss", Locale.getDefault())
    return sdf.format(Date())
}

class MqttManager(val onLog: (String) -> Unit) {
    private lateinit var client: MqttClient
    private val serverIP = "tcp://54.180.119.169" // 포트 명시 (기본 MQTT 포트: 1883)
    private val clientId = "ANDROIDCONTROLLER"
    private val topic = "rpi/motor"

    fun connect() {
        try {
            // MemoryPersistence를 사용하여 클라이언트 상태를 메모리에 저장
            client = MqttClient(serverIP, clientId, MemoryPersistence())
            val options = MqttConnectOptions()
            options.isCleanSession = true

            client.connect(options)

            val logMsg = "[${currentTimeString()}] 연결 성공 (IP: ${serverIP}, TOPIC: ${topic})"
            onLog(logMsg)
        } catch (e: Exception) {
            val logMsg = "[${currentTimeString()}] 연결 실패 (${e.message})"
            onLog(logMsg)
            e.printStackTrace()
        }
    }

    fun publish(message: String) {
        if (::client.isInitialized && client.isConnected) {
            try {
                val mqttMessage = MqttMessage(message.toByteArray())
                client.publish(topic, mqttMessage)
                val logMsg = "[${currentTimeString()}] 메시지 발행: $message"
                onLog(logMsg)
            } catch (e: Exception) {
                val logMsg = "[${currentTimeString()}] 메시지 발행 실패: ${e.message}"
                onLog(logMsg)
                e.printStackTrace()
            }
        } else {
            val logMsg = "[${currentTimeString()}] 연결되지 않음. 발행 실패"
            onLog(logMsg)
        }
    }

    // 연결 해제 메서드 (필요 시 호출)
    fun disconnect() {
        if (::client.isInitialized && client.isConnected) {
            try {
                client.disconnect()
                val logMsg = "[${currentTimeString()}] 연결 해제 성공"
                onLog(logMsg)
            } catch (e: Exception) {
                val logMsg = "[${currentTimeString()}] 연결 해제 실패: ${e.message}"
                onLog(logMsg)
                e.printStackTrace()
            }
        }
    }
}