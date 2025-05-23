import paho.mqtt.client as mqtt
import firebase_admin
from firebase_admin import credentials, firestore
from datetime import datetime, timedelta, timezone

# 한국 시간대 정의
KST = timezone(timedelta(hours=9))

# Firebase 초기화
cred = credentials.Certificate("/home/ubuntu/firebase/rc-car-pjt-firebase-key.json")
firebase_admin.initialize_app(cred)
db = firestore.client()

# float 변환 함수 (불가능하면 None 반환)
def parse_sensor_value(value):
    try:
        return float(value)
    except (ValueError, TypeError):
        return None

# MQTT 연결 콜백
def on_connect(client, userdata, flags, rc):
    print("MQTT 연결됨. 코드:", rc)
    topics = ["exp/data", "rover/data"]
    for topic in topics:
        client.subscribe(topic)
        print("구독 시작:", topic)

# MQTT 메시지 수신 콜백
def on_message(client, userdata, msg):
    try:
        topic = msg.topic
        payload = msg.payload.decode('utf-8').strip()
        now = datetime.now(KST)
        time_str = now.strftime("%Y-%m-%d_%H:%M:%S")
        doc_id = f"data_{time_str}"

        # 슬래시로 구분하고 누락된 값은 None으로 보완
        values = payload.split("/")
        while len(values) < 5:
            values.append(None)

        temp = parse_sensor_value(values[0]) if values[0] is not None else None
        humi = parse_sensor_value(values[1]) if values[1] is not None else None
        gas  = parse_sensor_value(values[2]) if values[2] is not None else None
        pres = parse_sensor_value(values[3]) if values[3] is not None else None
        mois = parse_sensor_value(values[4]) if values[4] is not None else None

        data = {
            "temp": temp,
            "humi": humi,
            "gas": gas,
            "pres" : pres,
            "mois" : mois,
            "time": now,
        }

        if topic == "exp/data":
            db.collection("EXP").document(doc_id).set(data)
        elif topic == "rover/data":
            db.collection("ROVER").document(doc_id).set(data)
        else:
            print("알 수 없는 토픽:", topic)

        print(f"[{topic}] Firestore 저장 완료: {doc_id} (temp={temp}, humi={humi}, gas={gas}, pressure={pres}, moisture={mois})")

    except Exception as e:
        print("예외 발생:", e)

# MQTT 클라이언트 실행
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("localhost", 1883, 60)
client.loop_forever()
