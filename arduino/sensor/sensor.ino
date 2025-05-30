#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <WiFi.h>


// ── Wi-Fi & MQTT 설정 ───────────────────────────────────
const char* ssid       = "LJH";
const char* password   = "00000000";
const char* mqttServer = "54.180.119.169";
const uint16_t mqttPort= 1883;
const char*  mqttTopic = "rover/data";

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

#define BMP280_I2C_ADDRESS 0x76  
Adafruit_BMP280 bmp;

// ── 전송 간격 ───────────────────────────────────────────
unsigned long lastPublish = 0;
const unsigned long interval = 5 * 1000;  // 2초마다 측정·전송

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

void setup() {
  Serial.begin(115200);

  // DHT 내부 풀업
  pinMode(DHTPIN, INPUT_PULLUP);
  dht.begin();

  // MQ-2, Soil AO는 입력 모드 선언 불필요(analogRead)
  // Soil DO 미사용

  // BMP280, IC3 통신 세팅
  Wire.begin(21, 22, 100000);  

  if (!bmp.begin(BMP280_I2C_ADDRESS)) {
    Serial.println("ERROR: BMP280 not found. Check wiring & address.");
    while (1) delay(10);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                Adafruit_BMP280::SAMPLING_X2,
                Adafruit_BMP280::SAMPLING_X16,
                Adafruit_BMP280::FILTER_X16,
                Adafruit_BMP280::STANDBY_MS_500);

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

  // ── 온·습도, 기압 측정 ─────────────────────────────────────
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();
  float pres = bmp.readPressure() / 100.0F;

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("❌ DHT read failed");
    return;
  }

  if (isnan(pres)) {
    Serial.println("❌ BMP read failed");
    return;
  }

  // ── 가스 센서(MQ-2) 측정 ─────────────────────────────
  int   gasRaw     = analogRead(MQ2PIN);  
  float gasVoltage = gasRaw; // * (3.3f / 4095.0f);

  // ── 토양 습도 측정 ───────────────────────────────────
  int rawSoil      = analogRead(SOIL_AO_PIN);
  int soilPct      = map(rawSoil, SOIL_DRY_VALUE, SOIL_WET_VALUE, 0, 100);
  soilPct = constrain(soilPct, 0, 100);

  // ── JSON 페이로드 생성 ───────────────────────────────
  char payload[256];
  int len = snprintf(payload, sizeof(payload),
    "%.1f/%.1f/%.2f/%.2f/%d",
    temperature, humidity, gasVoltage, pres, soilPct   // 4번째 값(기압)은 임시로 0.0 사용
  );

  // ── MQTT 퍼블리시 ───────────────────────────────────
  if (mqtt.publish(mqttTopic, payload, len)) {
    Serial.print("Published: ");
    Serial.println(payload);
  } else {
    Serial.println("❌ Publish failed");
  }
}