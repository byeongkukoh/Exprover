import time
import base64

import cv2                          # OpenCV 라이브러리 (JPEG 인코딩)
import paho.mqtt.client as mqtt     # 파이썬용 MQTT 클라이언트 라이브러리

from picamera2 import Picamera2     # PiCamera2 제어 라이브러리


# MQTT 설정
MQTT_BROKER = "54.180.119.169"      # MQTT Broker IP
MQTT_PORT = 1883                    # MQTT Port (기본: 1883)
MQTT_TOPIC = "rpi/image"            # MQTT TOPIC


# Picamera2 설정
picam2 = Picamera2()
picam2.preview_configuration.main.size = (320, 240)         # 해상도 설정
picam2.preview_configuration.main.format = "RGB888"         # 색상 포맷 설정 (R: 8bit, G: 8bit, B: 8Bit)
picam2.configure("preview")
picam2.start()                                              # 카메라 캡처 시작


# MQTT 클라이언트 생성 및 연결
client = mqtt.Client()                          # MQTT Client 객체 생성

def connect_mqtt():
    while True:
        try:
            client.connect(MQTT_BROKER, MQTT_PORT, 60)      # MQTT Broker 연결
            print(f"[MQTT] Connected to broker (ip: {MQTT_BROKER})")
            break
        except Exception as err:
            print(f"[MQTT] Connection failed: {err}")
            time.sleep(5)       # MQTT 재연결 시도 간격

connect_mqtt()


# MQTT 전송 실행
try:
    while True:
        frame = picam2.capture_array()
        
        # JPEG로 압축 (압축률 40 = 저화질)
        _, buffer = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 40])
        jpg_base64 = base64.b64encode(buffer).decode('utf-8')
        
        # MQTT 전송 시도
        try:
            result = client.publish(MQTT_TOPIC, jpg_base64)
            status = result[0]
            if status == 0:
                print("[MQTT] Image sent successfully")
            else:
                print(f"[MQTT] Failed to send image (status: {status}), reconnecting...")
                connect_mqtt()  # MQTT 연결 재시도

        except Exception as err:
            print(f"[MQTT] Publish exception: {err}, reconnecting...")
            connect_mqtt()  # MQTT 연결 재시도
        
        time.sleep(0.03)  # 30ms (약 33fps)

except KeyboardInterrupt:
    print("종료 중...")

finally:
    picam2.stop()
    client.disconnect()
