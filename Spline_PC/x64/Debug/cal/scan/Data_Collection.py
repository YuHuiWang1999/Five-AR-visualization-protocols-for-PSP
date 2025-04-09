'''此程序采用多进程同时采集US图像和NDI位姿，通过queue通讯，开启程序后c截屏，q退出，可用脚踏控制，注意更改脚踏按键'''
import time
from multiprocessing import Process, Queue
import cv2
import keyboard
from . import Get_Camera
from . import Get_NDI

# if __name__ == '__main__':
def scan_us(us_probe,dir):
    frame_queue = Queue()
    pos_queue = Queue()
    save_index = 0

    # us_probe = 1# 选择标定对象
    if us_probe:# 图像标定
        tracker_process = Process(target=Get_NDI.open_NDI, args=(dir,pos_queue, save_index, "tantou", True))  # 0是超声探头，1是光学探针
        capture_process = Process(target=Get_Camera.open_Camera, args=(dir,frame_queue, 1, save_index))  # 0是本机摄像头，1是图像采集卡
    else:       # 体模标定
        tracker_process = Process(target=Get_NDI.open_NDI, args=(dir,pos_queue, save_index, "tanzhen", False))
        capture_process = Process(target=Get_Camera.open_Camera, args=(dir,frame_queue, -1, save_index))
    capture_process.start()
    tracker_process.start()

    loop = True
    count = 0  # 脚踏控制计数
    delay = 0.1  # 延迟补偿
    while loop:
        if keyboard.is_pressed('c'):
            cv2.waitKey(200)
            pos_queue.put_nowait("c")
            time.sleep(delay)
            frame_queue.put_nowait("c")

        if keyboard.is_pressed("q"):
            cv2.waitKey(200)
            print("\n主程序退出")
            pos_queue.put_nowait("q")
            time.sleep(delay)
            frame_queue.put_nowait("q")
            loop = False
            break

        if not loop:
            capture_process.terminate()
            tracker_process.terminate()
