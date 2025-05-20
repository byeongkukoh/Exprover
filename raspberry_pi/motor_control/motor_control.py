import time

from .Raspi_MotorHAT import Raspi_MotorHAT, Raspi_DCMotor
from .Raspi_PWM_Servo_Driver import PWM

from mqtt.mqtt_client import MQTTClient

# Motor 초기 설정
motorHat = Raspi_MotorHAT(addr=0x6f) 
dcMotor = motorHat.getMotor(2) #핀번호
svMotor = PWM(address = 0x6F)

dcMotorSpeed = 100
dcMotor.setSpeed(dcMotorSpeed) #속도
svMotor.setPWMFreq(60)

def GO():
	svMotor.setPWM(0, 0, 350)
	dcMotor.run(motorHat.FORWARD)


def LEFT():
	svMotor.setPWM(0, 0, 200)


def RIGHT():
	svMotor.setPWM(0, 0, 500)


def BACK():
	svMotor.setPWM(0, 0, 350)
	dcMotor.run(motorHat.BACKWARD)


def STOP():
    global dcMotorSpeed
    dcMotorSpeed = 100
    dcMotor.setSpeed(dcMotorSpeed)
    dcMotor.run(motorHat.RELEASE)


def ACC():
    global dcMotorSpeed
    if (dcMotorSpeed < 200):
        dcMotorSpeed += 10

    dcMotor.setSpeed(dcMotorSpeed)
    

# 명령어 모음
command_map = {
    "go": GO,
    "back": BACK,
    "left": LEFT,
    "right": RIGHT,
    "stop": STOP,
    "acc": ACC,
}

# callback 함수 정의
def handle_command(topic, message):
    print(f"[MQTT] Received: {message}")
    action = command_map.get(message.strip().lower())

    if action:
        action()
    else:
        print(f"[WARN] Unknown command: {message}")

def run_motor_control():
    mqtt = MQTTClient()
    mqtt.connect()
    mqtt.subscribe("motor", handle_command)

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("[Main] Ctrl+C 입력됨. MQTT 종료")
        mqtt.disconnect()