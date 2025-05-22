package com.rccar.androidcontroller

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.material.Button
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp

class MainActivity : ComponentActivity() {
    private lateinit var mqttManager: MqttManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        try {
            mqttManager = MqttManager()
            mqttManager.connect() // Context 제거
            Log.d("MainActivity", "MQTT 연결 시도")
        } catch (e: Exception) {
            Log.e("MainActivity", "MQTT 연결 실패: ${e.message}", e)
        }

        setContent {
            MainScreen(mqttManager)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        try {
            mqttManager.disconnect() // 연결 해제
            Log.d("MainActivity", "MQTT 연결 해제")
        } catch (e: Exception) {
            Log.e("MainActivity", "MQTT 연결 해제 실패: ${e.message}", e)
        }
    }
}

// 메인 화면 구성
@Composable
fun MainScreen(mqttManager: MqttManager) {
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .padding(8.dp),
        verticalArrangement = Arrangement.Top,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Spacer(modifier = Modifier.height(20.dp))
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .aspectRatio(4f / 3f)
                .background(Color.Black),
            contentAlignment = Alignment.Center
        ) {
            Text("영상 출력 영역", color = Color.White)
        }
        Spacer(modifier = Modifier.height(32.dp))
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .aspectRatio(4f / 2f)
                .background(Color.Black),
            contentAlignment = Alignment.Center
        ) {
            Text("로그 출력 영역", color = Color.White)
        }
        Spacer(modifier = Modifier.height(32.dp))
        ControlPanel(mqttManager)
    }
}

// 컨트롤 패널 구성
@Composable
fun ControlPanel(mqttManager: MqttManager) {
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .padding(8.dp),
        horizontalArrangement = Arrangement.SpaceEvenly,
        verticalAlignment = Alignment.CenterVertically
    ) {
        Column(
            modifier = Modifier.width(IntrinsicSize.Min),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            DirectionGrid(mqttManager = mqttManager)
        }
        Column(
            modifier = Modifier.width(IntrinsicSize.Min),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Button(
                onClick = { Log.d("ControlPanel", "Accelation 버튼 눌림") },
                modifier = Modifier
                    .width(128.dp)
                    .height(64.dp)
                    .padding(4.dp)
            ) {
                Text("Accelation")
            }
            Button(
                onClick = { mqttManager.publish("stop") }, // Brake 버튼에 stop 명령 추가
                modifier = Modifier
                    .width(128.dp)
                    .height(64.dp)
                    .padding(4.dp)
            ) {
                Text("Brake")
            }
        }
    }
}

// 방향키 그리드 구성
@Composable
fun DirectionGrid(modifier: Modifier = Modifier, mqttManager: MqttManager) {
    Column(
        modifier = modifier.padding(8.dp),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Row(horizontalArrangement = Arrangement.Center) {
            Spacer(modifier = Modifier.size(64.dp))
            DirectionButton("UP", mqttManager)
            Spacer(modifier = Modifier.size(64.dp))
        }
        Row(horizontalArrangement = Arrangement.Center) {
            DirectionButton("LF", mqttManager)
            DirectionButton("DW", mqttManager)
            DirectionButton("RG", mqttManager)
        }
    }
}

// 방향키 버튼 함수
@Composable
fun DirectionButton(label: String, mqttManager: MqttManager) {
    Button(
        onClick = {
            val command = when (label) {
                "UP" -> "go"
                "DW" -> "back"
                "LF" -> "left"
                "RG" -> "right"
                else -> ""
            }
            mqttManager.publish(command)
            Log.d("DirectionButton", "버튼 클릭: $command")
        },
        modifier = Modifier
            .size(64.dp)
            .padding(4.dp)
    ) {
        Text(
            label,
            fontSize = 14.sp,
            fontWeight = FontWeight.W900,
            textAlign = TextAlign.Center
        )
    }
}