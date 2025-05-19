# 📷 Raspberry Pi Camera Live Streaming

라즈베리파이의 PiCamera2로부터 실시간으로 영상을 캡쳐하고, 이를 JPEG 이미지로 변환하여 MQTT 프로토콜을 통해 전송하는 프로그램

## 🧑‍💻 기술 스택

![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-C51A4A?style=for-the-badge&logo=raspberrypi&logoColor=white) 
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)

### 라이브러리
![OpenCV](https://img.shields.io/badge/OpenCV-5C3EE8?style=for-the-badge&logo=opencv&logoColor=white) 
![paho-mqtt](https://img.shields.io/badge/paho--mqtt-000000?style=for-the-badge&logo=eclipse&logoColor=white)

## 🛠️ 세팅 방법

### 라이브러리 설치

1. `opencv-python` 설치
```bash
pip install opencv-python --break-system-packages
```

2. `paho-mqtt` 설치
```bash
pip install paho-mqtt --break-system-packages
```

3. `picamera2` 설치
```bash
sudo apt install python3-picamera2
```