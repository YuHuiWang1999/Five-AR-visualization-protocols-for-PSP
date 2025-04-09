import glob
import numpy as np
import torch
import os
import cv2
from unet_model import UNet
import skimage
# import skimage.morphology import skeletonize

def instantiate(dir1):
    dir11 = dir1.replace('\\', '/')
    if not os.path.exists(dir11):
        print("未发现权重路径")
        return
    print("权重路径为", dir11)
    # device
    global device
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    global net
    # 加载网络，图片为单通道，分类为1
    net = UNet(n_channels = 1, n_classes = 1)
    # 将网络拷贝至device中
    net.to(device = device)
    # 加载模型参数
    net.load_state_dict(torch.load(dir11, map_location = device))
    # 测试模式
    net.eval()
    print("权重加载完成")
    print("模型加载完成")
    return device, net


def seg_group(dir2):
    # 确保使用正斜杠
    dir2 = dir2.replace('\\', '/')
    # 判断图像文件是否存在
    if not os.path.exists(dir2):
        print("未发现图像路径")
        return
    print("图像路径为", dir2)
    # 定义保存路径
    save_img_dir = './predict/image_predict'
    save_txt_dir = './predict/label_txt'

    # 如果目录不存在则创建
    os.makedirs(save_img_dir, exist_ok=True)
    os.makedirs(save_txt_dir, exist_ok=True)

    # 获取目录下所有的.png文件
    img_files = [f for f in os.listdir(dir2) if f.endswith('.png')]

    for img_file in img_files:
        # 构造图片的完整路径
        img_path = os.path.join(dir2, img_file)

        # 定义保存的结果路径
        save_res_path = os.path.join(save_img_dir, img_file.split('.')[0] + '_res.png')
        save_txt_path = os.path.join(save_txt_dir, img_file.split('.')[0] + '.txt')

        # 读取图片
        img = cv2.imread(img_path)

        # 转为灰度图
        img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

        # 重新调整形状为(batch, channels, height, width)的格式
        img = img.reshape(1, 1, img.shape[0], img.shape[1])

        # 转换为tensor
        img_tensor = torch.from_numpy(img).to(device=torch.device('cuda' if torch.cuda.is_available() else 'cpu'),
                                              dtype=torch.float32)

        # 使用模型进行预测
        pred = net(img_tensor)

        # 提取预测结果
        pred = np.array(pred.data.cpu()[0])[0]

        # 对预测结果进行阈值处理：大于等于0.5的区域设为255，其他区域设为0
        pred[pred >= 0.5] = 255
        pred[pred < 0.5] = 0


        pred = skimage.morphology.skeletonize(pred)
        pred = pred.astype(np.uint8) * 255
        # pred = morphology(pred)
        # skeleton = skeletonize(img_binary)

        # 保存结果图像
        cv2.imwrite(save_res_path, pred)

        # 获取白色区域的像素坐标
        white_pixels = np.argwhere(pred == 255)

        # 保存坐标到文本文件
        with open(save_txt_path, 'w') as f:
            for row in white_pixels:
                f.write(f"{row[0]} {row[1]}\n")




