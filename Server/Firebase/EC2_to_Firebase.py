import paho.mqtt.client as mqtt
import firebase_admin
from firebase_admin import credentials, firestore
import json
from datetime import datetime

# Firebase 초기화
cred = credentials.Certificate("/home/ubuntu/firebase/rc-car-pjt-firebase-key.json")  # 실제 경로로 수정
firebase_admin.initialize_app(cred)
db = firestore.client()

# MQTT 연결 콜백
def on_connect(client, userdata, flags, rc):
    print("MQTT 연결됨. 코드:", rc)
    topics = ["esp32/temp", "esp32/humi", "esp32/gas"]
    for topic in topics:
        client.subscribe(topic)
        print("구독 시작:", topic)

# MQTT 메시지 수신 콜백
def on_message(client, userdata, msg):
    try:
        topic = msg.topic
        payload = msg.payload.decode('utf-8')

        data_value = float(payload)

        now = datetime.now()
        time_str = now.strftime("%Y-%m-%d_%H:%M:%S")

        if topic == "esp32/temp":
            data = {
                "temp": data_value,
                "time": now,
            }
            doc_id = f"temp_{time_str}"
            db.collection("TEMPERATURE").document(doc_id).set(data)

        elif topic == "esp32/humi":
            data = {
                "humi": data_value,
                "time": now,
            }
            doc_id = f"humi_{time_str}"
            db.collection("HUMIDITY").document(doc_id).set(data)

        elif topic == "esp32/gas":
            data = {
                "gas": data_value,
                "time": now,
            }
            doc_id = f"gas_{time_str}"
            db.collection("GAS CONCENTRATION").document(doc_id).set(data)

        print(f"Firestore 저장 완료: {doc_id}")

    except Exception as e:
        print("오류 발생:", e)

# MQTT 클라이언트 실행
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("localhost", 1883, 60)
client.loop_forever()

