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

class MqttManager(
    private val onLog: (String) -> Unit,
    private val onConnectionStatusChanged: (Boolean) -> Unit
) {
    private lateinit var client: MqttClient
    private val serverIP = "tcp://54.180.119.169:1883" // 포트 명시
    private val clientId = "ANDROIDCONTROLLER_${System.currentTimeMillis()}" // 고유 클라이언트 ID
    private var motorTopic: String = ""
    private var videoTopic: String = ""

    fun connect(carName: String) {
        motorTopic = "$carName/motor"
        videoTopic = "$carName/video"

        try {
            if (::client.isInitialized && client.isConnected) {
                disconnect() // 기존 연결 해제
            }

            client = MqttClient(serverIP, clientId, MemoryPersistence())
            val options = MqttConnectOptions()
            options.isCleanSession = true

            // 메시지 수신 콜백 설정
            client.setCallback(object : MqttCallback {
                override fun connectionLost(cause: Throwable?) {
                    val logMsg = "[${currentTimeString()}] 연결 끊김: ${cause?.message ?: "알 수 없는 이유"}"
                    onLog(logMsg)
                    onConnectionStatusChanged(false)
                }

                override fun messageArrived(topic: String, message: MqttMessage) {
                    val payload = String(message.payload)
                    val logMsg = "[${currentTimeString()}] 메시지 수신 ($topic): $payload"
                    onLog(logMsg)
                    if (topic == videoTopic) {
                        onLog("[${currentTimeString()}] 비디오 데이터 수신: $payload")
                        // TODO: 비디오 데이터 처리 (예: UI에 표시)
                    }
                }

                override fun deliveryComplete(token: IMqttDeliveryToken?) {
                    // 사용 안 함
                }
            })

            client.connect(options)
            onConnectionStatusChanged(true)

            // 토픽 구독
            client.subscribe(motorTopic, 1)
            client.subscribe(videoTopic, 1)

            val logMsg = "[${currentTimeString()}] 연결 성공 (IP: $serverIP, TOPIC: $motorTopic, $videoTopic)"
            onLog(logMsg)
        } catch (e: Exception) {
            val logMsg = "[${currentTimeString()}] 연결 실패 (${e.message})"
            onLog(logMsg)
            onConnectionStatusChanged(false)
            e.printStackTrace()
        }
    }

    fun publish(message: String) {
        if (::client.isInitialized && client.isConnected) {
            try {
                val mqttMessage = MqttMessage(message.toByteArray())
                client.publish(motorTopic, mqttMessage)
                val logMsg = "[${currentTimeString()}] 메시지 발행 ($motorTopic): $message"
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

    fun disconnect() {
        if (::client.isInitialized && client.isConnected) {
            try {
                client.disconnect()
                onConnectionStatusChanged(false)
                val logMsg = "[${currentTimeString()}] 연결 해제 성공"
                onLog(logMsg)
            } catch (e: Exception) {
                val logMsg = "[${currentTimeString()}] 연결 해제 실패: ${e.message}"
                onLog(logMsg)
                e.printStackTrace()
            }
        }
    }

    fun logMessage(message: String) {
        onLog(message)
    }
}