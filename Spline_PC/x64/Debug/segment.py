import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('TkAgg')
import cv2
import time
import os
import shutil
from PIL import Image
import glob
import torchvision.transforms as transforms
from natsort import natsorted
from skimage import morphology
import numpy as np
import torch
from model import UNet

def instantiate(dir11):
    
    global start_time
    start_time = time.time()

    dir1 = dir11.replace('\\', '/')
    if not os.path.exists(dir1):
        print("未发现权重路径")
        return
    print("权重路径为", dir1)

    # device
    global device
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    global net
    net = UNet(1,1).to(device)
    print("模型实例化完成")
    # 恢复权重
    # 加载进来的模型可以用于测试,也可以继续进行训练
    net.load_state_dict(torch.load(dir1))
    # model.load_state_dict(torch.load("A:/deep_mrs/saved_model/Bone0.75.pt"))
    print("权重加载完成")
    print("模型加载完成")
    print('当前segment_api耗时{}'.format(time.time() - start_time))

    return device,net 

# =====================================================================================================================================================
def segment_api_single(dir22,index):
    size = index[2] - index[0]
    data = str(time.strftime("%Y%m%d", time.localtime()))
    dir2 = dir22.replace('\\', '/')
    # 判断图像文件是否存在
    if not os.path.exists(dir2):
        print("未发现图像路径")
        return
    print("图像路径为", dir2)


    dir2_u = dir2.rsplit('/', 2)[0]

    dir2_1 = dir2_u + '/pre'
    dir2_2 = dir2_u + '/txt'
    dir2_3 = dir2_u + '/sim'
    if not os.path.exists(dir2_1):
        os.mkdir(dir2_1)
    if not os.path.exists(dir2_2):
        os.mkdir(dir2_2)
    if not os.path.exists(dir2_3):
        os.mkdir(dir2_3)

    print("进入分割程序")
    global start_time


    transform = transforms.Compose([
        transforms.Grayscale(num_output_channels=1),
        transforms.ToTensor(),
        transforms.Normalize(0, 1)])

    for i in range(1):
        fopen = Image.open(dir2)
        # 0,0表示要裁剪的位置的左上角坐标，size,size表示右下角
        # fopen = fopen.crop((0,0,size,size))



        # 预处理后超声
        img_input = transform(fopen).to(device, dtype=torch.float32)  # data就是预处理后，可以送入模型进行训练的数据了    
        
        # # 海信超声
        # fopen = fopen.crop((376,49,376+1080,49+1080))
        # # fopen = fopen.crop((49,376,49+1080,376+1080))        
        # plt.imshow(fopen)
        # plt.show()
        # img_input = transform(fopen)#.to(device, dtype=torch.float32)  # data就是预处理后，可以送入模型进行训练的数据了
        # img_input[:,226:256,:] = 0
        # plt.imshow(img_input.squeeze(0))
        # plt.show()
        # img_input = img_input.to(device, dtype=torch.float32)
        
        # # mindray超声
        # fopen = fopen.crop((323,94,323+421,94+421))      
        # plt.imshow(fopen)
        # plt.show()
        # img_input = transform(fopen)#.to(device, dtype=torch.float32)  # data就是预处理后，可以送入模型进行训练的数据了
        # plt.imshow(img_input.squeeze(0))
        # plt.show()
        # img_input = img_input.to(device, dtype=torch.float32) 



        output = net(img_input.unsqueeze(0))[0].detach().cpu().numpy()  # 放入模型进行测试
        y_p = output[0][0] * 255
        img_b = cv2.threshold(y_p, 200, 255, cv2.THRESH_BINARY)

        # 元组类型，第一个元素为‘阈值’，第二个元素为图像数组
        # 提取图像数组
        img_s = img_b[1]
        img_s[0:40, :] = 0

        name = dir2.rsplit('/', 1)[1].rsplit('.', 1)[0]
        cv2.imwrite("{}/img_{}.png".format(dir2_1,name), img_s)

        #----------------------------------------------------------------------------------
        img_s[img_s == 255] = 1
        skeleton0 = morphology.skeletonize(img_s)
        skeleton = skeleton0.astype(np.uint8) * 255

        cv2.imwrite("{}/sim_{}.png".format(dir2_3,name), skeleton)
        
        #-------------------------
        #---------------------------------------------------------


        with open("{}/txt_{}.txt".format(dir2_2,name), 'w') as f:

            for p1 in range(skeleton.shape[0]):
                for p2 in range(skeleton.shape[1]):
                    if skeleton[p1][p2] == 255:
                       f.write("{} {}\n".format(p1, p2))

        print("数据成功预测")
        print('当前segment_api耗时{}'.format(time.time() - start_time))

    print("程序运行结束")
    print('当前segment_api耗时{}'.format(time.time() - start_time))

    return 0

def segment_api_group(dir22,index):

    size = index[2] - index[0]
    data = str(time.strftime("%Y%m%d", time.localtime()))
    dir2 = dir22.replace('\\', '/')
    # 判断图像文件是否存在
    if not os.path.exists(dir2):
        print("未发现图像路径")
        return
    print("图像路径为", dir2)


    dir2_u = dir2.rsplit('/', 1)[0]

    dir2_1 = dir2_u + '/pre'
    dir2_2 = dir2_u + '/txt'
    dir2_3 = dir2_u + '/sim'

    if not os.path.exists(dir2_1):
        os.mkdir(dir2_1)
    if not os.path.exists(dir2_2):
        os.mkdir(dir2_2)
    if not os.path.exists(dir2_3):
        os.mkdir(dir2_3)


    print("进入分割程序")
    global start_time


    transform = transforms.Compose([
        transforms.Grayscale(num_output_channels=1),
        transforms.ToTensor(),
        transforms.Normalize(0, 1)])

    dir_c = dir2 + "/*"
    img_list = natsorted(glob.glob(dir_c))
    img_len = len(img_list)

    for i in range(img_len):

        fopen = Image.open(img_list[i])
        # 0,0表示要裁剪的位置的左上角坐标，size,size表示右下角
        # fopen = fopen.crop((0,0,size,size))



		# 预处理后超声
        img_input = transform(fopen).to(device, dtype=torch.float32)  # data就是预处理后，可以送入模型进行训练的数据了    
        
        # # 海信超声
        # fopen = fopen.crop((376,49,376+1080,49+1080))
        # # fopen = fopen.crop((49,376,49+1080,376+1080))        
        # plt.imshow(fopen)
        # plt.show()
        # img_input = transform(fopen)#.to(device, dtype=torch.float32)  # data就是预处理后，可以送入模型进行训练的数据了
        # img_input[:,226:256,:] = 0
        # plt.imshow(img_input.squeeze(0))
        # plt.show()
        # img_input = img_input.to(device, dtype=torch.float32)
        
        # # mindray超声
        # fopen = fopen.crop((323,94,323+421,94+421))      
        # plt.imshow(fopen)
        # plt.show()
        # img_input = transform(fopen)#.to(device, dtype=torch.float32)  # data就是预处理后，可以送入模型进行训练的数据了
        # # img_input[:,226:256,:] = 0
        # plt.imshow(img_input.squeeze(0))
        # plt.show()
        # img_input = img_input.to(device, dtype=torch.float32) 



        output = net(img_input.unsqueeze(0))[0].detach().cpu().numpy()  # 放入模型进行测试
        y_p = output[0][0] * 255
        img_b = cv2.threshold(y_p, 200, 255, cv2.THRESH_BINARY)

        # 元组类型，第一个元素为‘阈值’，第二个元素为图像数组
        # 提取图像数组
        img_s = img_b[1]
        img_s[0:40, :] = 0

        name = img_list[i].rsplit('\\', 1)[1].rsplit('.', 1)[0]
        cv2.imwrite("{}/img_{}.png".format(dir2_1,name), img_s)

        #----------------------------------------------------------------------------------
        img_s[img_s == 255] = 1
        skeleton0 = morphology.skeletonize(img_s)
        skeleton = skeleton0.astype(np.uint8) * 255

        cv2.imwrite("{}/sim_{}.png".format(dir2_3,name), skeleton)
     
        #-------------------------
        #---------------------------------------------------------


        with open("{}/txt_{}.txt".format(dir2_2,name), 'w') as f:

            for p1 in range(skeleton.shape[0]):
                for p2 in range(skeleton.shape[1]):
                    if skeleton[p1][p2] == 255:
                       f.write("{} {}\n".format(p1, p2))

        print("数据成功预测" + str(i+1) + "/" + str(img_len))
        print('当前segment_api耗时{}'.format(time.time() - start_time))

    print("程序运行结束")
    print('当前segment_api耗时{}'.format(time.time() - start_time))

    return 0