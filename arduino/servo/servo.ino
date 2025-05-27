#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// Wi-Fi 정보
const char* ssid = "bk-wifi";
const char* password = "byeongkukoh1007";

// Raspberry Pi의 IP 주소
const char* mqtt_server = "54.180.119.169";

WiFiClient espClient;
PubSubClient client(espClient);

Servo armServo;     // 기존 서보모터 (pin 18)
Servo soilServo;    // 새 서보모터 (pin 17)

const int servoPin = 18;
const int soilPin = 17;

void setup_wifi() {
  delay(10);
  Serial.println("WiFi connet...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(" WiFi connet complete!");
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim();

  if (msg == "hi") {
    greetMotion();
  } else if (msg == "soil") {
    soilMotion();
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      client.subscribe("exp/motor");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  armServo.setPeriodHertz(50);
  armServo.attach(servoPin, 500, 2400);
  armServo.write(0);  // 시작 위치

  soilServo.setPeriodHertz(50);
  soilServo.attach(soilPin, 500, 2400);
  soilServo.write(90);  // 기준점 (0도라고 생각)

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// 인사 동작
void greetMotion() {
  armServo.write(90); delay(500);
  for (int i = 0; i < 3; i++) {
    armServo.write(95); delay(300);
    armServo.write(65); delay(300);
  }
  armServo.write(0); delay(500);
}

// soil 메시지에 대한 동작
void soilMotion() {
  soilServo.write(45);   // 90 기준에서 왼쪽(-30도)
  delay(2000);           // 2초 대기
  soilServo.write(90);   // 기준 위치로 복귀 (0도)
  delay(500);
}
