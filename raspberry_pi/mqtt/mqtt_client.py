import time
import paho.mqtt.client as mqtt

# 설정: 브로커와 포트
MQTT_BROKER_IP = "54.180.119.169"
MQTT_PORT = 1883    # 기본 포트: 1883

# 토픽 정의
TOPICS = {
    "video": "rpi/video",
    "motor": "rpi/motor",
}


# MQTT Client Class 정의
class MQTTClient:
    # MQTT 객체 초기화
    def __init__(self, client_id="", broker_ip=MQTT_BROKER_IP, port=1883):
        self.broker_ip = broker_ip
        self.port = port
        self.client = mqtt.Client(client_id = client_id)

    # MQTT 연결 시도
    def connect(self):
        while True:
            try:
                self.client.connect(self.broker_ip, self.port, 60)  # Broker 연결
                print(f"[MQTT] Connected to {self.broker_ip}: {self.port}")
                break
            except Exception as err:
                print(f"[MQTT] Connection failed: {err}")
                time.sleep(5)   # 5초 후 MQTT 재연결 시도

    # MQTT 발행 (Publish)
    def publish(self, topic_key, message):
        topic = TOPICS.get(topic_key)

        if topic is None:
            print(f"[MQTT] Unknown topic key: {topic_key}")
            return

        try:
            result = self.client.publish(topic, message)

            if result[0] == 0:
                print(f"[MQTT] Published to {topic}")
            else:
                print(f"[MQTT] Failed to publish to {topic} (status: {result[0]})")

        except Exception as err:
            print(f"[MQTT] Publish error: {err}")
            self.connect() # 재연결 시도

    # MQTT 구독 (Subscribe)
    def subscribe(self, topic_key, callback):
        topic =  TOPICS.get(topic_key)
        if topic is None:
            print(f"[MQTT] Unknown topic key: {topic_key}")
            return
        
        def on_message(client, userdata, msg):
            callback(msg.topic, msg.payload.decode())
        
        self.client.on_message = on_message
        self.client.subscribe(topic)
        self.client.loop_start()    # 백그라운드 수신 루프 실행

    # 연결 해제
    def disconnect(self):
        self.client.disconnect()

    # 루프 중단 방지
    def loop_forever(self):
        self.client.loop_forever()
