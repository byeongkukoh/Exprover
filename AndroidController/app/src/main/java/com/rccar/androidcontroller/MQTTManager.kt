package com.rccar.androidcontroller

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.util.Base64
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
    private val onConnectionStatusChanged: (Boolean) -> Unit,
    private val onVideoFrameReceived: (Bitmap?) -> Unit = {}
) {
    private lateinit var client: MqttClient
    private val serverIP = "tcp://54.180.119.169:1883"
    private val clientId = "ANDROIDCONTROLLER_${System.currentTimeMillis()}"
    private var motorTopic: String = ""
    private var videoTopic: String = ""

    fun connect(carName: String) {
        motorTopic = "$carName/motor"
        videoTopic = "$carName/video"
        try {
            if (::client.isInitialized && client.isConnected) {
                disconnect()
            }
            client = MqttClient(serverIP, clientId, MemoryPersistence())
            val options = MqttConnectOptions()
            options.isCleanSession = true
            options.keepAliveInterval = 60
            options.isAutomaticReconnect = true

            client.setCallback(object : MqttCallback {
                override fun connectionLost(cause: Throwable?) {
                    val logMsg = "[${currentTimeString()}] 연결 끊김: ${cause?.message ?: "알 수 없는 이유"}"
                    onLog(logMsg)
                    onConnectionStatusChanged(false)
                }

                override fun messageArrived(topic: String, message: MqttMessage) {
                    val payload = String(message.payload)
                    val logMsg = "[${currentTimeString()}] 메시지 수신 ($topic): $payload"
                    // onLog(logMsg) // 디버깅용 로그, 필요 시 활성화
                    if (topic == videoTopic) {
                        try {
                            val decodedBytes = Base64.decode(payload, Base64.DEFAULT)
                            val bitmap = BitmapFactory.decodeByteArray(decodedBytes, 0, decodedBytes.size)
                            onVideoFrameReceived(bitmap)
                            // onLog("[${currentTimeString()}] 비디오 프레임 처리 성공: ${bitmap.width}x${bitmap.height}") // 디버깅용 로그
                        } catch (e: Exception) {
                            onLog("[${currentTimeString()}] 비디오 데이터 처리 실패: ${e.message}")
                            onVideoFrameReceived(null)
                        }
                    }
                }

                override fun deliveryComplete(token: IMqttDeliveryToken?) {}
            })

            client.connect(options)
            onConnectionStatusChanged(true)
            client.subscribe(videoTopic, 1)
            client.subscribe(motorTopic, 1)
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
        onLog("publish 호출 - motorTopic: $motorTopic")
        onLog("client 상태 - 초기화: ${::client.isInitialized}, 연결: ${if (::client.isInitialized) client.isConnected else false}")
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
            val logMsg = "[${currentTimeString()}] 연결되지 않음. 발행 실패 (motorTopic: $motorTopic)"
            onLog(logMsg)
            if (::client.isInitialized && !client.isConnected) {
                onLog("[${currentTimeString()}] 연결 재시도")
                try {
                    client.reconnect()
                } catch (e: Exception) {
                    onLog("[${currentTimeString()}] 연결 재시도 실패: ${e.message}")
                }
            }
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
        } else {
            val logMsg = "[${currentTimeString()}] 이미 연결되지 않음"
            onLog(logMsg)
        }
    }

    fun logMessage(message: String) {
        onLog(message)
    }
}