import time
import base64

import cv2                          # OpenCV 라이브러리 (JPEG 인코딩)

from picamera2 import Picamera2     # PiCamera2 제어 라이브러리

from mqtt.mqtt_client import MQTTClient

def run_camera_streaming():
    # MQTT 클라이언트 연결
    mqtt = MQTTClient()
    mqtt.connect()


    # Picamera2 설정
    picam2 = Picamera2()
    picam2.preview_configuration.main.size = (320, 240)         # 해상도 설정
    picam2.preview_configuration.main.format = "RGB888"         # 색상 포맷 설정 (R: 8bit, G: 8bit, B: 8Bit)
    picam2.configure("preview")
    picam2.start()                                              # 카메라 캡처 시작


    # MQTT 전송 실행
    try:
        while True:
            frame = picam2.capture_array()
            
            # JPEG로 압축 (압축률 40 = 저화질)
            _, buffer = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 40])
            jpg_base64 = base64.b64encode(buffer).decode('utf-8')

            mqtt.publish("video", jpg_base64)
            time.sleep(0.1)    # 100ms (약 10fps)

    except KeyboardInterrupt:
        print("종료 중...")

    finally:
        picam2.stop()
        mqtt.disconnect()
