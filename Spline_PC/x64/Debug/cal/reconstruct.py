import cv2
import numpy as np
import torch
import torchvision.transforms as transforms
from .rebuild.model import Mrs_net
from skimage import morphology
import time
import re
import glob
from natsort import natsorted
import os
from .src.save import save_pic



def main(dir,order=34):
    def read_pics(image_filenames):
        image_list = natsorted(glob.glob(image_filenames))

        for i, image_name in enumerate(image_list):
            frame = cv2.imread(image_name)
            width, height = frame.shape[0], frame.shape[1]
            # 降采样至模型训练时接受数据的尺寸down_frame
            down_frame = cv2.resize(frame, (256, 256))
            tensor_frame = transform(down_frame).to(device)

            # 神经网络分割输入数据model_frame
            model_frame = model(tensor_frame.unsqueeze(0))[0].detach().cpu().numpy()  # 放入模型进行测试

            # 分割结果恢复至原尺寸up_frame
            up_frame = cv2.resize(model_frame[0][0], (height, width))

            # 分割掩膜二值化threshold_frame，并提取单像素宽度骨架提取single_frame
            threshold_frame = cv2.threshold(up_frame, 0.8, 1, cv2.THRESH_BINARY)
            threshold_frame = threshold_frame[1]
            single_frame = morphology.skeletonize(threshold_frame)

            # 消除上方常见伪影
            out_frame = single_frame.astype(np.uint8) * 255
            out_frame[0:20, :] = 0

            # print("the shape of out_frame:", out_frame.shape)

            # 回调函数
            def on_mouse_click(event):
                # 检查是否是右键点击
                if event == cv2.EVENT_RBUTTONDOWN:
                    selected = True
                    print(f"Selected: {selected}")
                if event == cv2.EVENT_LBUTTONDOWN:
                    selected = False
                    print(f"Selected: {selected}")

            # 读取图片
            def show_image(img1, img2):
                if img2 is not None:
                    img1 = cv2.resize(img1, (500, 500))[:, :, 0]
                    img2 = cv2.resize(img2, (500, 500))
                    # 显示图片时，文件名作为窗口名传递，用于鼠标回调
                    img = np.concatenate((img1, img2), axis=0)
                    cv2.imshow("image", img)
                    cv2.setMouseCallback(on_mouse_click)
                    cv2.waitKey(0)
                    cv2.destroyAllWindows()
                return selected

            selected = True
            # selected = show_image(frame,threshold_frame)

            if selected:
                file_path = dir+"/rebuild/pics/point{}.txt".format(i)
                # 打开文件以写入模式
                with open(file_path, 'w') as file:
                    coordinates = np.where(out_frame == 255)
                    for j in range(len(coordinates[0])):
                        x, y = coordinates[0][j], coordinates[1][j]
                        file.write("{},{}".format(x, y))
                        # file.write("{},{}".format(y, x))
                        file.write('\n')

                cv2.imwrite("rebuild/pics/image{}.png".format(i), frame)
                cv2.imwrite("rebuild/pics/mask{}.png".format(i), out_frame)

    # 读取ndi下的探头工具架和体模工具架位姿
    def read_ndi(coordinate_filename, mode):
        us_position = []

        # 读取txt文件
        with open(coordinate_filename, 'r') as file:
            data = file.read()

        # 替换nan值
        data = data.replace("nan", "0")

        data = data.replace("\n", "")
        # 使用正则表达式作为分割符，以数字为分隔符
        result = re.split(r'\d+\.\s', data)
        result.pop(0)

        for index in result:
            if 'nan' in index:
                continue
            index = eval(index)
            if mode == "image":
                us_position.append(index[0])
            elif mode == "video":
                us_position.append(index)

        row = len(us_position)
        us_position = np.array(us_position).reshape(row, 4, 4)

        # print("the shape of us_pose:")
        # print(us_position.shape)
        # print(us_position)

        return us_position

    def point_trans(pic_index, calibration_matrixs, us_poses):
        # print(pic_index)
        pic_points = dir+"/rebuild/pics/point{}.txt".format(pic_index)
        # 读取txt文件
        with open(pic_points, 'r') as file:
            lines = file.readlines()
            line_count = len(lines)
            data = np.zeros((line_count, 4))

            for index_row, line in enumerate(lines):
                line = line.strip()  # 移除每行的换行符和空格
                if line:
                    # 将每行以制表符或空格分割为数字，然后转换为浮点数
                    row = [float(num) for num in line.split(',')]  # 使用空格分隔符，你可以根据需要更改
                    data[index_row][0] = row[0]
                    data[index_row][1] = row[1]
                    data[index_row][2] = 0
                    data[index_row][3] = 1
        # print("the shape of data:")
        # print(data.shape)
        # print(data)

        transformed_points = us_poses[pic_index] @ calibration_matrixs @ data.T
        transformed_points = transformed_points.T
        # print("the shape of transformed_points:")
        # print(transformed_points.shape)
        # print(transformed_points)
        return transformed_points

    transform = transforms.Compose([transforms.ToTensor(), ])
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    print("torch.cuda.is_available()", torch.cuda.is_available())
    model = Mrs_net(16, 'BeltBlock', True).to(device)
    checkpoint = torch.load(dir + "/rebuild/now_pred.pth.tar")['state_dict']
    model.load_state_dict(checkpoint)

    start_time = time.time()
    cut_filenames = dir + r"\wts\cut_index.txt"
    cut_index = np.loadtxt(cut_filenames, delimiter=',').astype(int)

    order = order
    image_filenames = dir + r"\imgs/raw{}/imgs".format(order)
    save_pic(image_filenames, order, cut_index,dir)
    cut_filenames = dir + r"\imgs/raw{}/imgs/*.jpg".format(order)

    pose_filenames = dir + r"\data\ndi_read{}.txt".format(order)
    matrix_filenames = dir + r"\imgs\matrix.txt"
    with open(matrix_filenames, 'r') as file:
        lines = file.readlines()
        calibration_matrix = [list(eval(line)) for line in lines]
        line_count = len(lines)

    # 分割图像，提取坐标（平面内）===========================================================================================
    if not os.path.exists(dir + "/rebuild/pics"):
        os.makedirs(dir + "/rebuild/pics")
    read_pics(cut_filenames)
    # 提取每张图像对应的探头位姿 video / image
    us_pose = read_ndi(pose_filenames, "image")

    save_path = dir + r"\wts\3d_coordinate{}.txt".format(order)
    with open(save_path, 'w') as file:
        # 每张图像
        for num in range(0, len(us_pose)):
            point_clouds = point_trans(num, calibration_matrix, us_pose)
            # 每张图像内的每个点
            for j in range(len(point_clouds)):
                # if j%100 == 0:
                # 间隔采样
                if j % 100 == 0:
                    x, y, z = point_clouds[j][0], point_clouds[j][1], point_clouds[j][2]
                    if x == 0 and y == 0 and z == 0:
                        continue
                    file.write("{} {} {}".format(x, y, z))
                    file.write('\n')
                else:
                    pass

    print("数据成功预测")
    print('当前segment_api耗时{}'.format(time.time() - start_time))

if __name__ == "__main__":
    main()
