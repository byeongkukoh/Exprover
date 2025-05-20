import paho.mqtt.client as mqtt
import subprocess

from Raspi_MotorHAT import Raspi_MotorHAT, Raspi_DCMotor
from Raspi_PWM_Servo_Driver import PWM
from time import sleep

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
    

# MQTT 콜백 함수 정의
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe("rpi/command")


def on_message(client, userdata, msg):
    command = msg.payload.decode()
    print(f"Received command: {command}")
    
    try:
        if (command == "go"):
            GO()
        elif (command == "back"):
            BACK()
        elif (command == "right"):
            RIGHT()
        elif (command == "left"):
            LEFT()
        elif (command == "stop"):
            STOP()
        elif (command == "acc"):
            ACC()
        else:
            print("Wrong Command. (go, back, left, right, stop)")

    except subprocess.CalledProcessError as e:
        print(f"Command failed:\n{e.stderr}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("54.180.119.169", 1883, 60)
client.loop_forever()

