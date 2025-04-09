import torch
import torch.nn as nn
import torch.nn.functional as F

class DoubleConv(nn.Module):
    def __init__(self, in_channels, out_channels):
        super().__init__()
        # inplace = True 时,会修改输入对象的值,所以打印出对象存储地址相同,类似于C语言的址传递，不会创建新的对象
        self.double_conv = nn.Sequential(
            nn.Conv2d(in_channels, out_channels, kernel_size = 3, padding = 1),
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace = True),
            nn.Conv2d(out_channels, out_channels, kernel_size = 3, padding = 1),
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace = True)
        )

    def forward(self, x):
        return self.double_conv(x)

class Down(nn.Module):
    # 通过最大池化降采样，随后两次卷积
    def __init__(self, in_channels, out_channels):
        super().__init__()
        self.maxpool_conv = nn.Sequential(
            nn.MaxPool2d(2),
            DoubleConv(in_channels, out_channels)
    )

    def forward(self, x):
        return self.maxpool_conv(x)

class Up(nn.Module):
    # 上采样，随后两次卷积
    # 如果bilinear参数为True，则使用双线性插值的方式进行上采样(使用常规卷积减少通道数),否则使用反卷积的方式进行上采样
    def __init__(self, in_channels, out_channels, bilinear = True):
        # 在添加子类特有的功能之前，初始化父类中定义的属性和执行其他必要的设置
        super().__init__()
        self.conv1x1 = nn.Conv2d(in_channels, out_channels, kernel_size = 1)
        if bilinear:
            self.up = nn.Upsample(scale_factor = 2, mode = 'bilinear', align_corners = True)
        else:
            self.up = nn.ConvTranspose2d(in_channels // 2, in_channels // 2, kernel_size = 2, stride = 2)
        self.conv = DoubleConv(in_channels, out_channels)

    def forward(self, x1, x2):
        x1 = self.up(x1)
        x1 = self.conv1x1(x1)
        diffY = torch.tensor([x2.size()[2] - x1.size()[2]])
        diffX = torch.tensor([x2.size()[3] - x1.size()[3]])
        x1 = F.pad(x1, [diffX // 2, diffX - diffX // 2, diffY // 2, diffY - diffY // 2])
        x = torch.cat([x2, x1], dim = 1)
        return self.conv(x)

class OutConv(nn.Module):
    def __init__(self, in_channels, out_channels):
        super(OutConv, self).__init__()
        self.conv = nn.Conv2d(in_channels, out_channels, kernel_size = 1)

    def forward(self, x):
        return self.conv(x)













