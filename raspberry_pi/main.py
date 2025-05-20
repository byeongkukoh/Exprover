import multiprocessing
from camera_streaming.video_live_streaming import run_camera_streaming
from motor_control.motor_control import run_motor_control

def main():
    proc1 = multiprocessing.Process(target = run_camera_streaming)
    proc2 = multiprocessing.Process(target = run_motor_control)

    proc1.start()
    proc2.start()

    proc1.join()
    proc2.join()

if __name__ == "__main__":
    main()