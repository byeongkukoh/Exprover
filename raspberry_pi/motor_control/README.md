# 🚗 Raspberry Pi Motor Control

## 🧑‍💻 기술 스택

![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-C51A4A?style=for-the-badge&logo=raspberrypi&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)

### 라이브러리

![raspi-motorhat](https://img.shields.io/badge/raspi--motorhat-00979D?style=for-the-badge&logo=raspberrypi&logoColor=white)
![paho-mqtt](https://img.shields.io/badge/paho--mqtt-000000?style=for-the-badge&logo=eclipse&logoColor=white)

## 🛠️ 세팅 방법

### 라이브러리 설치

1. `Raspi-MotorHat` 설치

   - UGEEK MotorHat 라이브러리 설치 ([GitHub](https://github.com/casmer/ugeek-stepper-motor-hat/))

     ```bash
     git clone https://github.com/casmer/ugeek-stepper-motor-hat.git
     ```

   - 클론한 디렉토리로 이동 후, 아래 3개 파일을 작업 디렉토리로 복사
   - `Raspi_MotorHat.py`, `Raspi_PWM_Servo_Driver.py`, `Raspi_I2C.py`

     ```bash
     cd ugeek-stepper-motor-hat
     cp Raspi_*.py ~/project_dir
     ```

2. `paho-mqtt` 설치

   ```bash
   pip install paho-mqtt --break-system-packages
   ```
