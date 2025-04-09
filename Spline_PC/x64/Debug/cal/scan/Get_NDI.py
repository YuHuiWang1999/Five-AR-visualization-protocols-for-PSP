import time

import numpy as np
from sksurgerynditracker.nditracker import NDITracker
import time
from .Check_Filename import check_filename


def open_NDI(dir,queue, save_dir, symbol="tantou", us=True):
    rom_list = {
        "tantou": dir+"/rom/MRS_tantou.rom",
        "tanzhen": dir+"/rom/MRS_tanzhen.rom",  # 探头
        "timo": dir+"/rom/MRS_timo.rom",  # 探针
    }
    settings_vega = {
        "tracker type": "vega",
        # "ip address": "169.254.219.146",
        "ip address": "169.254.34.43",
        "port": 8765,
        "romfiles": [
            rom_list[symbol],
            rom_list["timo"],  # 体模
        ]
    }

    tracker = NDITracker(settings_vega)
    # tracker.use_quaternions = True    # 输出四元数
    tracker.start_tracking()
    print(tracker.get_tool_descriptions())
    if us:
        pic_name_backup = check_filename(dir+'/data/ndi_read{}.txt'.format(save_dir))  # 截图功能
        pic_name = check_filename(dir+'/data/backup/ndi_read{}.txt'.format(save_dir))  # 截图功能
    else:
        pic_name = check_filename(dir+'/data/points_in_phantom.txt')  # 截图功能
    print('NDI初始化完成')
    pic_position = []
    while True:
        port, time_stamps, frame_numbers, tracking, tracking_quality = tracker.get_frame()
        time.sleep(1 / 60)
        # print("pic_position********2")
        try:
            signal = queue.get(block=False)
        except:
            signal = " "
        if signal == "q":
            print("POS退出/停止录制")
            break
        if signal == "c":  # 截图功能
            pic_position.append(np.array(tracking).reshape(2, 16).tolist())  # 截图功能
            print("POS已截图")  # 截图功能

    tracker.stop_tracking()
    tracker.close()
    print("pic_position", pic_position)
    if pic_position:
        print("pic_position********3")
        with open(pic_name, 'w') as file:
            for i, item in enumerate(pic_position):
                file.write("{}. {}\n".format(i + 1, item))
        with open(pic_name_backup, 'w') as file:
            for i, item in enumerate(pic_position):
                file.write("{}. {}\n".format(i + 1, item))
