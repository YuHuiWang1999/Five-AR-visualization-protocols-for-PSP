'''----------------打开摄像头，按下w录屏，按下q退出，剪裁到US框----------------'''
import os

import cv2
import time
from .Check_Filename import check_filename


def open_Camera(dir,queue, symbol, save_dir):
    if symbol == -1:
        while 1:
            try:
                signal = queue.get(block=False)
            except:
                signal = " "
            if signal == "q":
                print("IMG退出/停止录制")
                break
    cap = cv2.VideoCapture(symbol)  # 摄像头对象为0或1
    if not cap.isOpened():
        print("无法打开摄像头")
        exit()
    print('IMG相机初始化完成')

    # 定义一个起始和结束的数值范围
    start = 1
    end = 9  # 你可以根据需要修改这个值
    # 创建一个空列表来保存文件名
    file_names = []

    # 外层循环控制主要数字（1, 2, ...）
    for main_number in range(start, end + 1):
        # 内层循环控制次要数字（1, 2, ..., 9）
        for sub_number in range(1, 10):
            # 格式化文件名并添加到列表中
            file_name = f"{main_number}_{sub_number}"
            file_names.append(file_name)

    status = False
    index_pic = 0
    while cap.isOpened():
        ret, frame = cap.read()
        # frame = frame[94:526, 323:765, :]  # 迈瑞：裁剪94-525,323-764
        # frame = frame[70:570, 169:795, :]    # 海信：裁剪70-569,169-794

        now = time.time()
        try:
            signal = queue.get(block=False)
        except:
            signal = " "

        if signal == "q":
            print("IMG退出/停止录制")
            break
        if signal == "c":  # 截图功能
            if not status:
                pic_name = check_filename(dir+"/imgs/raw{}".format(save_dir))  # 截图功能
                os.makedirs(pic_name + "/imgs/")
                status = True
            cv2.imwrite(pic_name + r"/imgs/{}.jpg".format(file_names[index_pic]), frame)  # 截图功能
            print("IMG已截图")  # 截图功能
            index_pic += 1

        cv2.waitKey(1)
        cv2.imshow('US VideoCapture', frame)

    cap.release()  # 释放摄像头
    cv2.destroyAllWindows()
