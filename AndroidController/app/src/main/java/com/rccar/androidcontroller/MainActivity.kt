package com.rccar.androidcontroller

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background

import androidx.compose.runtime.Composable
import androidx.compose.material.Text
import androidx.compose.material.Button
import androidx.compose.foundation.layout.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.*


class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            MainScreen()
        }
    }
}

// 메인 화면 구성
@Composable
fun MainScreen() {
    Column (
        modifier = Modifier
            .fillMaxWidth().padding(8.dp),
        verticalArrangement = Arrangement.Top,
        horizontalAlignment = Alignment.CenterHorizontally,
    ) {
        Spacer(modifier = Modifier.height(20.dp))
        Box(
            modifier = Modifier
                .fillMaxWidth().aspectRatio(4f / 3f).background(Color.Black),
            contentAlignment = Alignment.Center
        ) {
            Text("영상 출력 영역", color = Color.White)
        }
        Spacer(modifier = Modifier.height(32.dp))
        Box(
            modifier = Modifier
                .fillMaxWidth().aspectRatio(4f / 2f).background(Color.Black),
            contentAlignment = Alignment.Center
        ) {
            Text("로그 출력 영역", color = Color.White)
        }
        Spacer(modifier = Modifier.height(32.dp))
        ControlPanel()
    }
}

// 컨트롤 패널 구성
@Composable
fun ControlPanel() {
    Row(
        modifier = Modifier
            .fillMaxWidth().padding(8.dp),
        horizontalArrangement = Arrangement.SpaceEvenly,
        verticalAlignment = Alignment.CenterVertically,
    ) {
        Column(
            modifier = Modifier.width(IntrinsicSize.Min),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally,
        ) {
            DirectionGrid()
        }
        Column (
            modifier = Modifier.width(IntrinsicSize.Min),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally,
        ) {
            Button(
                onClick = { println("눌림")},
                modifier = Modifier
                    .width(128.dp).height(64.dp).padding(4.dp),
            ) {
                Text("Accelation")
            }
            Button(
                onClick = { println("눌림")},
                modifier = Modifier
                    .width(128.dp).height(64.dp).padding(4.dp),
            ) {
                Text("Brake")
            }
        }
    }
}

// 방향키 그리드 구성
@Composable
fun DirectionGrid(modifier: Modifier = Modifier) {
    Column(
        modifier = modifier
            .padding(8.dp),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally,
    ) {
        Row (
            horizontalArrangement = Arrangement.Center,
        ) {
            Spacer(modifier = Modifier.size(64.dp))
            DirectionButton("UP")
            Spacer(modifier = Modifier.size(64.dp))
        }
        Row (
            horizontalArrangement = Arrangement.Center,
        ) {
            DirectionButton("LF")
            DirectionButton("DW")
            DirectionButton("RG")
        }
    }
}

// 방향키 버튼 함수
@Composable
fun DirectionButton(label: String) {
    Button(
        onClick = { println("$label 눌림")},
        modifier = Modifier
            .size(64.dp).padding(4.dp),
    ) {
        Text(
            label,
            fontSize = 14.sp,
            fontWeight = FontWeight.W900,
            textAlign = TextAlign.Center
        )
    }
}