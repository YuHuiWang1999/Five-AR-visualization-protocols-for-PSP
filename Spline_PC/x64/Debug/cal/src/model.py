import torch
import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import GCNConv, TopKPooling


# 图卷积模块
class UNetGraphConv(nn.Module):
    def __init__(self, in_channels, out_channels):
        super(UNetGraphConv, self).__init__()
        self.conv = GCNConv(in_channels, out_channels)

    def forward(self, x, edge_index):
        x = F.relu(self.conv(x, edge_index))
        return x


# 卷积模块
class UNetConv(nn.Module):
    def __init__(self, in_channels, out_channels):
        super(UNetConv, self).__init__()
        self.conv = torch.nn.Sequential(
            torch.nn.Conv2d(in_channels=in_channels, out_channels=out_channels,
                            kernel_size=3, padding=1, stride=1,
                            bias=False),
            torch.nn.BatchNorm2d(out_channels),
            torch.nn.ReLU(inplace=True), )

    def forward(self, x):
        x = self.conv(x)
        return x


# 上采样
class Up(nn.Module):
    def __init__(self, in_channels):
        super().__init__()
        self.up = nn.ConvTranspose2d(in_channels, in_channels, kernel_size=2, stride=2)
        self.conv = torch.nn.Sequential(
            torch.nn.Conv2d(in_channels=in_channels * 2, out_channels=in_channels, kernel_size=3, padding=1, stride=1,
                            bias=False),
            torch.nn.BatchNorm2d(in_channels),
            torch.nn.ReLU(inplace=True),

            torch.nn.Conv2d(in_channels=in_channels, out_channels=in_channels // 2, kernel_size=3, padding=1, stride=1,
                            bias=False),
            torch.nn.BatchNorm2d(in_channels // 2),
            torch.nn.ReLU(inplace=True),
        )

    def forward(self, x1, x2):
        x1 = self.up(x1)
        x = torch.cat([x2, x1], dim=1)
        return self.conv(x)

# 图-卷积分割模型的输入是3*
class Mymodel(nn.Module):
    def __init__(self):
        super(Mymodel, self).__init__()
        self.encode_g1 = UNetGraphConv(3, 32)
        self.encode_c1 = UNetConv(3, 32)
        self.down_1 = TopKPooling(64, ratio=0.25)

        self.encode_g2 = UNetGraphConv(64, 64)
        self.encode_c2 = UNetConv(64, 64)
        self.down_2 = TopKPooling(128, ratio=0.25)

        self.encode_g3 = UNetGraphConv(128, 128)
        self.encode_c3 = UNetConv(128, 128)
        self.down_3 = TopKPooling(256, ratio=0.25)

        self.encode_g4 = UNetGraphConv(256, 256)
        self.encode_c4 = UNetConv(256, 256)
        self.down_4 = TopKPooling(512, ratio=0.25)

        self.neck = UNetGraphConv(512, 512)
        self.cneck = UNetConv(512, 512)

        self.decode_1 = Up(512)
        self.decode_2 = Up(256)
        self.decode_3 = Up(128)
        self.decode_4 = Up(64)

        self.conv = torch.nn.Conv2d(in_channels=32, out_channels=1, kernel_size=1, padding=0, stride=1)
        self.sigmoid = torch.nn.Sigmoid()

    def forward(self, data, topk_batch, data_batch, img_size=128):
        # batch0为批次大小
        x, edge_index = data.x, data.edge_index
        encode_g1 = self.encode_g1(x, edge_index)
        encode_c1 = self.encode_c1(x.reshape(data_batch, img_size, img_size, 3).permute(0, 3, 1, 2)).permute(0, 2, 3,
                                                                                                         1).reshape(-1,
                                                                                                                    32)
        encode_1 = torch.cat([encode_g1, encode_c1], dim=1)
        down_1, edge_index_1, _, batch_topk1, _, _ = self.down_1(encode_1, edge_index, None, topk_batch)

        encode_g2 = self.encode_g2(down_1, edge_index_1)
        encode_c2 = self.encode_c2(
            down_1.reshape(data_batch, img_size // 2, img_size // 2, 64).permute(0, 3, 1, 2)).permute(0, 2, 3, 1).reshape(
            -1, 64)
        encode_2 = torch.cat([encode_g2, encode_c2], dim=1)
        down_2, edge_index_2, _, batch_topk2, _, _ = self.down_2(encode_2, edge_index_1, None, batch_topk1)

        encode_g3 = self.encode_g3(down_2, edge_index_2)
        encode_c3 = self.encode_c3(
            down_2.reshape(data_batch, img_size // 4, img_size // 4, 128).permute(0, 3, 1, 2)).permute(0, 2, 3, 1).reshape(
            -1, 128)
        encode_3 = torch.cat([encode_g3, encode_c3], dim=1)
        down_3, edge_index_3, _, batch_topk3, _, _ = self.down_3(encode_3, edge_index_2, None, batch_topk2)

        encode_g4 = self.encode_g4(down_3, edge_index_3)
        encode_c4 = self.encode_c4(
            down_3.reshape(data_batch, img_size // 8, img_size // 8, 256).permute(0, 3, 1, 2)).permute(0, 2, 3, 1).reshape(
            -1, 256)
        encode_4 = torch.cat([encode_g4, encode_c4], dim=1)
        down_4, edge_index_4, _, batch_topk4, _, _ = self.down_4(encode_4, edge_index_3, None, batch_topk3)

        neck = self.neck(down_4, edge_index_4)
        # cneck = self.cneck(down_4.reshape(batch0,8,8,512).permute(0, 3, 1, 2)).permute(0, 2, 3, 1).reshape(-1,512)
        # neck = torch.cat([gneck, cneck], dim=1)

        neck = neck.view(data_batch, img_size // 16, img_size // 16, 512).permute(0, 3, 1, 2)
        encode_1 = encode_1.view(data_batch, img_size, img_size, 64).permute(0, 3, 1, 2)
        encode_2 = encode_2.view(data_batch, img_size // 2, img_size // 2, 128).permute(0, 3, 1, 2)
        encode_3 = encode_3.view(data_batch, img_size // 4, img_size // 4, 256).permute(0, 3, 1, 2)
        encode_4 = encode_4.view(data_batch, img_size // 8, img_size // 8, 512).permute(0, 3, 1, 2)

        upsample_1 = self.decode_1(neck, encode_4)
        upsample_2 = self.decode_2(upsample_1, encode_3)
        upsample_3 = self.decode_3(upsample_2, encode_2)
        upsample_4 = self.decode_4(upsample_3, encode_1)
        yout = self.conv(upsample_4)

        return self.sigmoid(yout)


if __name__ == '__main__':
    # 打印每一层的结构和参数数量
    total = 0
    model = Mymodel()
    for name, param in model.named_parameters():
        total = total + param.numel()
        # print(f'Layer: {name} | Size: {param.size()} | Parameters: {param.numel()} | Grad: {param.requires_grad}')
    print("Parameters is {} M".format(total/1000000))