# # 导入相关包
# # =====================================================================================================================================================
# import torch
# from torch import nn
# from torch.utils.data import Dataset,DataLoader
# import torchvision.datasets as tdst
# import torchvision.transforms as transforms
# import matplotlib.pyplot as plt
# import matplotlib
# matplotlib.use('TkAgg')
# import cv2
# import time
# import os
# import shutil
# from PIL import Image
# import glob
# import torchvision.transforms as transforms
# from natsort import natsorted
# from skimage import morphology
# import numpy as np
#
#
# # 定义网络结构
# # =====================================================================================================================================================
# class Senet(nn.Module):
#     def __init__(self, channel, ratio=16):
#         super(Senet, self).__init__()
#         self.avg_pool = nn.AdaptiveAvgPool2d(1)  # 因为是关注channel，这里将h和w压为1
#         self.fc = nn.Sequential(
#             nn.Linear(channel, channel // ratio, False),
#             nn.LeakyReLU(),
#             nn.Linear(channel // ratio, channel, False),
#             nn.Sigmoid(),
#         )
#
#     def forward(self, x):
#         b, c, h, w = x.size()
#         avg = self.avg_pool(x).view([b, c])  # 将压为1的h,w view
#         fc = self.fc(avg).view([b, c, 1, 1])  # 这里计算每一个channel 的w后，变回与input一样的size，便于点积
#         return x * fc
# class IRB(torch.nn.Module):
#     def __init__(self, in_channel, out_channel):
#         super().__init__()
#         self.step = torch.nn.Sequential(
#             torch.nn.Conv2d(in_channels=in_channel, out_channels=out_channel, kernel_size=1, padding=0, stride=1),
#             torch.nn.BatchNorm2d(out_channel),
#             torch.nn.LeakyReLU(),
#             torch.nn.Conv2d(in_channels=out_channel, out_channels=1, kernel_size=3, padding=1, stride=1, dilation=1,
#                             groups=1),
#             torch.nn.BatchNorm2d(1),
#             torch.nn.LeakyReLU(),
#             torch.nn.Conv2d(in_channels=1, out_channels=in_channel, kernel_size=1, padding=0, stride=1),
#             torch.nn.BatchNorm2d(in_channel),
#             torch.nn.LeakyReLU(),
#         )
#     def forward(self, x):
#         y = self.step(x)
#         return x + y
# class ConvBlock(torch.nn.Module):
#     def __init__(self, in_channels, out_channels):
#         super().__init__()
#         self.step = torch.nn.Sequential(
#             # 第一次卷积
#             torch.nn.Conv2d(in_channels=in_channels, out_channels=out_channels, kernel_size=3, padding=1, stride=1),
#             torch.nn.BatchNorm2d(out_channels),
#             torch.nn.Dropout2d(0.3),
#             # LeakyReLU
#             torch.nn.LeakyReLU(),
#
#             # # 第二次卷积
#             # torch.nn.Conv2d(in_channels=out_channels,out_channels=out_channels,kernel_size=3,padding=1,stride=1),
#             # torch.nn.BatchNorm2d(out_channels),
#             # torch.nn.Dropout2d(0.3),
#             # # LeakyReLU
#             # torch.nn.LeakyReLU()
#         )
#     def forward(self, x):
#         return self.step(x)
# class UNet(torch.nn.Module):
#     def __init__(self):
#         super().__init__()
#         C = 128
#         # 定义左侧编码器的操作
#         self.senet1_9 = Senet(C)
#         self.senet2_8 = Senet(2 * C)
#         self.senet3_7 = Senet(4 * C)
#         self.senet4_6 = Senet(8 * C)
#         # 实例化卷积块（完成每层的两次卷积）
#         self.layer1 = ConvBlock(1, C)
#         self.layer2 = ConvBlock(C, 2 * C)
#         self.layer3 = ConvBlock(2 * C, 4 * C)
#         self.layer4 = ConvBlock(4 * C, 8 * C)
#         self.layer5 = IRB(8 * C, 16 * C)
#         self.layer06 = ConvBlock(8 * C + 4 * C, 8 * C)
#         self.layer061 = ConvBlock(8 * C, 4 * C)
#         self.layer07 = ConvBlock(4 * C + 2 * C, 4 * C)
#         self.layer071 = ConvBlock(4 * C, 2 * C)
#         self.layer08 = ConvBlock(2 * C + 1 * C, 2 * C)
#         self.layer081 = ConvBlock(2 * C, 1 * C)
#         self.layer09 = ConvBlock(1 * C + C // 2, 1 * C)
#         self.layer091 = ConvBlock(1 * C, C // 2)
#         # 定义右侧解码器的操作
#         self.layer6 = torch.nn.ConvTranspose2d(in_channels=8 * C, out_channels=4 * C, kernel_size=2, stride=2,
#                                                bias=False)
#         self.layer7 = torch.nn.ConvTranspose2d(in_channels=4 * C, out_channels=2 * C, kernel_size=2, stride=2,
#                                                bias=False)
#         self.layer8 = torch.nn.ConvTranspose2d(in_channels=2 * C, out_channels=1 * C, kernel_size=2, stride=2,
#                                                bias=False)
#         self.layer9 = torch.nn.ConvTranspose2d(in_channels=C, out_channels=C // 2, kernel_size=2, stride=2, bias=False)
#         # 最后一个卷积
#         self.layer10 = torch.nn.Conv2d(in_channels=C // 2, out_channels=1, kernel_size=1, padding=0, stride=1)
#         # 定一些其他操作
#         # 池化
#         self.maxpool = torch.nn.MaxPool2d(kernel_size=2)
#         # 上采样,尺寸加倍（与通道数无关）
#         self.upsample = torch.nn.Upsample(scale_factor=2, mode='bilinear')
#         # sigmoid
#         self.sigmoid = torch.nn.Sigmoid()
#
#     def forward(self, x):
#         # 定义下采样部分
#         # input:1X256x256,  output: 64x256x256
#         x1 = self.layer1(x)
#         x1 = self.senet1_9(x1)
#         x1 = x1 + self.layer1(x)
#         # input:64x256x256, output: 64 x 128 x 128
#         x1_p = self.maxpool(x1)
#         # input:  64 x 128 x 128 , output: 128 x 128 x 128
#         x2 = self.layer2(x1_p)
#         x2 = self.senet2_8(x2)
#         x2 = x2 + self.layer2(x1_p)
#         # input:128 x 128 x 128 ,  output: 128 x 64 x 64
#         x2_p = self.maxpool(x2)
#         # input: 128 x 64 x 64, output: 256 x 64 x 64
#         x3 = self.layer3(x2_p)
#         x3 = self.senet3_7(x3)
#         x3 = x3 + self.layer3(x2_p)
#         # input:256 x 64 x 64,   output: 256 x 32 x 32
#         x3_p = self.maxpool(x3)
#         # input: 256 x 32 x 32, output: 512 x 32 x 32
#         x4 = self.layer4(x3_p)
#         x4 = self.senet4_6(x4)
#         x4 = x4 + self.layer4(x3_p)
#         # input:256 x 32 x 32,   output: 256 x 16 x 16
#         x4_p = self.maxpool(x4)
#         #####################################################
#         # 定义瓶颈层
#         # input: 256 x 16 x 16, output: 256 x 16 x 16
#         x5 = self.layer5(x4_p)
#         #####################################################
#         # 定义上采样
#         # input: 256 x 32 x 32,   output: 128 x 32 x 32
#         x6 = self.layer6(x5)
#         # 通道维（dim=1）拼接,   output: 256+128 x 32 x 32
#         x6 = torch.cat([x6, x4], dim=1)
#         # input: 256+128 x 32 x 32,   output: 128 x 32 x 32
#         x6 = self.layer06(x6)
#         x6 = self.layer061(x6)
#         # 定义上采样
#         # input: 128 x 32 x 32,   output: 64 x 32 x 32
#         x7 = self.layer7(x6)
#         # 通道维（dim=1）拼接,   output: 128+64 x 32 x 32
#         x7 = torch.cat([x7, x3], dim=1)
#         # input: 128+64 x 32 x 32,   output: 64 x 32 x 32
#         x7 = self.layer07(x7)
#         x7 = self.layer071(x7)
#         # 定义上采样
#         # input: 64 x 32 x 32,   output: 32 x 32 x 32
#         x8 = self.layer8(x7)
#         # 通道维（dim=1）拼接,   output: 64+32 x 32 x 32
#         x8 = torch.cat([x8, x2], dim=1)
#         # input: 64+32 x 32 x 32,   output: 32 x 32 x 32
#         x8 = self.layer08(x8)
#         x8 = self.layer081(x8)
#         # 定义上采样
#         # input: 32 x 32 x 32,   output: 16 x 32 x 32
#         x9 = self.layer9(x8)
#         # 通道维（dim=1）拼接,   output: 32+16 x 32 x 32
#         x9 = torch.cat([x9, x1], dim=1)
#         # input: 32+16 x 32 x 32,   output: 1 x 32 x 32
#         x9 = self.layer09(x9)
#         x9 = self.layer091(x9)
#         x10 = self.layer10(x9)
#         # sigmoid，img输出范围（0，1），与mask数据（0 or 1）相乘后计算dice面积
#         x11 = self.sigmoid(x10)
#         return x11
#
# # =====================================================================================================================================================
# def segment_api(dir11, dir22, index):
#     size = index[2] - index[0]
#     data = str(time.strftime("%Y%m%d", time.localtime()))
#
#     dir1 = dir11.replace('\\', '/')
#     if not os.path.exists(dir1):
#         print("未发现权重路径")
#         return
#     print("权重路径为", dir1)
#
#     dir2 = dir22.replace('\\', '/')
#     # 判断图像文件是否存在
#     if not os.path.exists(dir2):
#         print("未发现图像路径")
#         return
#
#     os.rename(dir2, dir2.rsplit('/', 1)[0] + "/img")
#     dir2 = dir2.rsplit('/', 1)[0] + "/img"
#
#     k = 1
#     dir2_u = dir2.rsplit('/', 1)[0] + '/' + data + "_" + str(k)
#     while os.path.exists(dir2_u):
#         k = k + 1
#         dir2_u = dir2.rsplit('/', 1)[0] + '/' + data + "_" + str(k)
#     dir2_u = dir2.rsplit('/', 1)[0] + '/' + data + "_" + str(k)
#     os.mkdir(dir2_u)
#
#     # print(dir2)
#     shutil.move(dir2, dir2_u)
#     dir_c = dir2_u + "/" + dir2.rsplit('/', 1)[1] + "/*"
#     print("图像路径为", dir2)
#
#     print("进入分割程序")
#     start_time = time.time()
#
#     # device
#     device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
#     model = UNet().to(device)
#     # 恢复权重
#     # 加载进来的模型可以用于测试,也可以继续进行训练
#     model.load_state_dict(torch.load(dir1))
#     # model.load_state_dict(torch.load("A:/deep_mrs/saved_model/Bone0.75.pt"))
#     print("权重加载完成")
#     print('当前segment_api耗时{}'.format(time.time() - start_time))
#
#     transform = transforms.Compose([
#         transforms.Grayscale(num_output_channels=1),
#         transforms.ToTensor(),
#         transforms.Normalize(0, 1)])
#
#     dir3_1 = dir2_u + '/pre'
#     dir3_2 = dir2_u + '/txt'
#     dir3_3 = dir2_u + '/sim'
#     os.mkdir(dir3_1)
#     os.mkdir(dir3_2)
#     os.mkdir(dir3_3)
#
#     img_list = natsorted(glob.glob(dir_c))
#     img_len = len(img_list)
#     for i in range(img_len):
#         fopen = Image.open(img_list[i])
#         # 0,0表示要裁剪的位置的左上角坐标，size,size表示右下角
#         # fopen = fopen.crop((0,0,size,size))
#
#         img_input = transform(fopen).to(device, dtype=torch.float32)  # data就是预处理后，可以送入模型进行训练的数据了
#         output = model(img_input.unsqueeze(0)).detach().cpu().numpy()  # 放入模型进行测试
#
#         # fig = plt.figure(figsize=(10, 10))
#         plt.subplot(1, 3, 1)
#         plt.imshow(img_input.detach().cpu().numpy().reshape(size, size), cmap="gray")
#         # plt.show()
#         plt.axis('off')
#         plt.subplot(1, 3, 2)
#         plt.imshow(output.reshape(size, size), cmap="gray")
#         # plt.show()
#         plt.axis('off')
#
#         y_p = output[0][0] * 255
#         img_b = cv2.threshold(y_p, 50, 255, cv2.THRESH_BINARY)
#
#         # 元组类型，第一个元素为‘阈值’，第二个元素为图像数组
#         # 提取图像数组
#         img_s = img_b[1]
#         name = img_list[i].rsplit('\\', 1)[1].rsplit('.', 1)[0]
#         cv2.imwrite(dir3_1 + "/{}.png".format(name), img_s)
#
#         #----------------------------------------------------------------------------------
#         img_s[img_s == 255] = 1
#         skeleton0 = morphology.skeletonize(img_s)
#         skeleton = skeleton0.astype(np.uint8) * 255
#         # cv2.imwrite("skeleton.png",skeleton)
#         plt.subplot(1, 3, 3)
#         plt.imshow(skeleton.reshape(size, size), cmap="gray")
#         plt.axis('off')
#         # plt.show()
#         cv2.imwrite(dir3_3 + "/{}.png".format(name), skeleton)
#         #-------------------------
#         #---------------------------------------------------------
#
#         with open(dir3_2 + "/{}.txt".format(name), 'w') as f:
#             for p1 in range(skeleton.shape[0]):
#                 for p2 in range(skeleton.shape[1]):
#                     if skeleton[p1][p2] == 255:
#                         f.write("{} {}\n".format(p1, p2))
#         print("数据成功预测" + str(i+1) + "/" + str(img_len))
#         print('当前segment_api耗时{}'.format(time.time() - start_time))
#
#
#     print("程序运行结束")
#     print('当前segment_api耗时{}'.format(time.time() - start_time))
#
#     return 0
#
# dir1 = r"B:\deep_mrs\saved_model\Bone0.75.pt"
# dir2 = r"B:\deep_mrs\uuss 0417\img"
# index = [0,0,448,448]
# segment_api(dir1,dir2,index)
# #