#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ESP32Servo.h>

// ── Wi-Fi & MQTT 설정 ───────────────────────────────────
const char* ssid       = "bk-wifi";
const char* password   = "byeongkukoh1007";
const char* mqttServer = "54.180.119.169";
const uint16_t mqttPort= 1883;
const char*  mqttTopic = "exp/date";

WiFiClient     wifiClient;
PubSubClient   mqtt(wifiClient);
Servo armServo;

// ── 센서 핀 설정 ────────────────────────────────────────
// DHT11 온·습도 센서
#define DHTPIN   13
#define DHTTYPE  DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ-2 가스 센서(AO)
#define MQ2PIN   35

// 토양 습도 센서(AO)
#define SOIL_AO_PIN    32
#define SOIL_DRY_VALUE 3000  // 흙이 건조할 때 raw 값
#define SOIL_WET_VALUE 1000  // 흙이 물에 잠겼을 때 raw 값

const int servoPin = 18;

// ── 전송 간격 ───────────────────────────────────────────
unsigned long lastPublish = 0;
const unsigned long interval = 10;  // 2초마다 측정·전송

// ── Wi-Fi 연결 ─────────────────────────────────────────
void connectWiFi() {
  Serial.printf("WiFi connecting to %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nWiFi connected, IP=%s\n", WiFi.localIP().toString().c_str());
}

// ── MQTT 연결 ───────────────────────────────────────────
void connectMQTT() {
  mqtt.setServer(mqttServer, mqttPort);
  while (!mqtt.connected()) {
    Serial.print("MQTT connecting…");
    if (mqtt.connect("ESP32_AllInOne")) {
      Serial.println(" connected!");
    } else {
      Serial.printf(" failed, rc=%d, retry in 2s\n", mqtt.state());
      delay(2000);
    }
  }
}

// -- 모터 조작 Callback 함수
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim();

  if (msg == "hi") {
    greetMotion();
  }
}

void reconnect() {
  while (!mqtt.connected()) {
    if (mqtt.connect("ESP32Client")) {
      mqtt.subscribe("exp/motor");
    } else {
      delay(2000);
    }
  }
}


void setup() {
  Serial.begin(115200);

  // DHT 내부 풀업
  pinMode(DHTPIN, INPUT_PULLUP);
  dht.begin();

  // MQ-2, Soil AO는 입력 모드 선언 불필요(analogRead)
  // Soil DO 미사용

  // 서보모터 세팅
  armServo.setPeriodHertz(50);
  armServo.attach(servoPin, 500, 2400);
  armServo.write(0);

  connectWiFi();
  connectMQTT();
  Serial.println("Setup complete");
}

void loop() {
  // 1) 네트워크·MQTT 유지
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected())           connectMQTT();
  mqtt.loop();

  // 2) 정해진 간격마다 측정 및 전송
  if (millis() - lastPublish < interval) return;
  lastPublish = millis();

  // ── 온·습도 측정 ─────────────────────────────────────
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("❌ DHT read failed");
    return;
  }

  // ── 가스 센서(MQ-2) 측정 ─────────────────────────────
  int   gasRaw     = analogRead(MQ2PIN);  
  float gasVoltage = gasRaw * (3.3f / 4095.0f);

  // ── 토양 습도 측정 ───────────────────────────────────
  int rawSoil      = analogRead(SOIL_AO_PIN);
  int soilPct      = map(rawSoil, SOIL_DRY_VALUE, SOIL_WET_VALUE, 0, 100);
  soilPct = constrain(soilPct, 0, 100);

  // ── JSON 페이로드 생성 ───────────────────────────────
  char payload[256];
  int len = snprintf(payload, sizeof(payload),
    "%.1f/%.1f/%.2f/%.2f/%d",
    temperature, humidity, gasVoltage, 0.0, soilPct   // 4번째 값(기압)은 임시로 0.0 사용
  );

  // ── MQTT 퍼블리시 ───────────────────────────────────
  if (mqtt.publish(mqttTopic, payload, len)) {
    Serial.print("Published: ");
    Serial.println(payload);
  } else {
    Serial.println("❌ Publish failed");
  }
}

void greetMotion() {
  armServo.write(90); delay(500);
  for (int i = 0; i < 3; i++) {
    armServo.write(110); delay(300);
    armServo.write(70); delay(300);
  }
  armServo.write(0); delay(500);
}
