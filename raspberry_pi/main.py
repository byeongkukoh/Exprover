import multiprocessing
from camera_streaming.video_live_streaming import run_camera_streaming
from motor_control.motor_control import run_motor_control

def main():
    name = input("차량 이름을 입력하세요: ").strip()
    proc1 = multiprocessing.Process(target = run_camera_streaming, args=(name,))
    proc2 = multiprocessing.Process(target = run_motor_control, args=(name,))

    proc1.start()
    proc2.start()

    try:
        proc1.join()
        proc2.join()
    except KeyboardInterrupt:
        print("\n[Main] KeyboardInterrupt 발생, 자식 프로세스 종료 중...")
        proc1.join()
        proc1.terminate()
        proc2.join()
        proc2.terminate()

if __name__ == "__main__":
    main()