package com.rccar.androidcontroller

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.material.Button
import androidx.compose.material.Text
import androidx.compose.material.TextField
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items

class MainActivity : ComponentActivity() {
    private lateinit var mqttManager: MqttManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            val logs = remember { mutableStateListOf<String>() }
            var carName by remember { mutableStateOf("") } // 차 이름 상태
            var isConnected by remember { mutableStateOf(false) } // 연결 상태

            // MQTT 매니저 초기화
            mqttManager = MqttManager(
                onLog = { msg -> logs.add(msg) },
                onConnectionStatusChanged = { connected -> isConnected = connected }
            )

            MainScreen(
                mqttManager = mqttManager,
                logs = logs,
                carName = carName,
                onCarNameChange = { carName = it },
                isConnected = isConnected
            )
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        try {
            mqttManager.disconnect()
            Log.d("MainActivity", "MQTT 연결 해제")
        } catch (e: Exception) {
            Log.e("MainActivity", "MQTT 연결 해제 실패: ${e.message}", e)
        }
    }
}

@Composable
fun MainScreen(
    mqttManager: MqttManager,
    logs: List<String>,
    carName: String,
    onCarNameChange: (String) -> Unit,
    isConnected: Boolean
) {
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .padding(8.dp),
        verticalArrangement = Arrangement.Top,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        // 차 이름 입력란과 연결 버튼
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(8.dp),
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically
        ) {
            TextField(
                value = carName,
                onValueChange = onCarNameChange,
                label = { Text("차 이름") },
                modifier = Modifier.weight(1f)
            )
            Spacer(modifier = Modifier.width(8.dp))
            Button(
                onClick = {
                    if (carName.isNotBlank()) {
                        mqttManager.connect(carName)
                    } else {
                        mqttManager.logMessage("[${currentTimeString()}] 차 이름을 입력하세요")
                    }
                },
                modifier = Modifier.width(100.dp)
            ) {
                Text(if (isConnected) "연결 해제" else "연결")
            }
        }
        // 연결 상태 표시
        Text(
            text = if (isConnected) "연결됨" else "연결 안됨",
            color = if (isConnected) Color.Green else Color.Red,
            modifier = Modifier.padding(8.dp)
        )
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
        LogBox(logs)
        Spacer(modifier = Modifier.height(32.dp))
        ControlPanel(mqttManager, isConnected)
    }
}

@Composable
fun ControlPanel(mqttManager: MqttManager, isConnected: Boolean) {
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
            DirectionGrid(mqttManager = mqttManager, enabled = isConnected)
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
                    .padding(4.dp),
                enabled = isConnected
            ) {
                Text("Accelation")
            }
            Button(
                onClick = { mqttManager.publish("stop") },
                modifier = Modifier
                    .width(128.dp)
                    .height(64.dp)
                    .padding(4.dp),
                enabled = isConnected
            ) {
                Text("Brake")
            }
        }
    }
}

@Composable
fun DirectionGrid(modifier: Modifier = Modifier, mqttManager: MqttManager, enabled: Boolean) {
    Column(
        modifier = modifier.padding(8.dp),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Row(horizontalArrangement = Arrangement.Center) {
            Spacer(modifier = Modifier.size(64.dp))
            DirectionButton("UP", mqttManager, enabled)
            Spacer(modifier = Modifier.size(64.dp))
        }
        Row(horizontalArrangement = Arrangement.Center) {
            DirectionButton("LF", mqttManager, enabled)
            DirectionButton("DW", mqttManager, enabled)
            DirectionButton("RG", mqttManager, enabled)
        }
    }
}

@Composable
fun DirectionButton(label: String, mqttManager: MqttManager, enabled: Boolean) {
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
            .padding(4.dp),
        enabled = enabled
    ) {
        Text(
            label,
            fontSize = 14.sp,
            fontWeight = FontWeight.W900,
            textAlign = TextAlign.Center
        )
    }
}

@Composable
fun LogBox(logs: List<String>, modifier: Modifier = Modifier) {
    Box(
        modifier = modifier
            .fillMaxWidth()
            .aspectRatio(4f / 2f)
            .background(Color.Black)
            .padding(8.dp)
    ) {
        LazyColumn {
            items(logs.reversed()) { log ->
                Text(log, color = Color.White, fontSize = 12.sp)
            }
        }
    }
}