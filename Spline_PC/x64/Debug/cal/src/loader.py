from torch.utils.data import Dataset
import torchvision.transforms as transforms
import numpy as np
import torch
import torchvision.datasets as tdst
from torch_geometric.data import Data


# 训练/测试阶段的卷积图像数据集制作==========================================================================================
class ConvDataset(Dataset):
    def __init__(self, root_dir, transform=None):
        super().__init__()
        # 返回数据集所在路径
        self.image_dir = root_dir + "/image"
        self.label_dir = root_dir + "/label"
        self.image_dataset = tdst.ImageFolder(root=self.image_dir, transform=transform)
        self.label_dataset = tdst.ImageFolder(root=self.label_dir, transform=transform)

    def __len__(self):
        assert len(self.image_dataset) == len(self.label_dataset), "数据集中的图像与标签不匹配"
        return len(self.image_dataset)

    def __getitem__(self, ind):
        return self.image_dataset[ind][0], self.label_dataset[ind][0]


# 推理阶段的卷积图像数据集制作==============================================================================================
class ConvDataset_inf(Dataset):
    def __init__(self, root_dir, transform=None):
        super().__init__()
        # 返回数据集所在路径
        self.image_dir = root_dir
        self.image_dataset = tdst.ImageFolder(root=self.image_dir, transform=transform)

    def __len__(self):
        return len(self.image_dataset)

    def __getitem__(self, ind):
        return self.image_dataset[ind][0], self.image_dataset[ind][0]


# 图结构数据的制作========================================================================================================
# ①构造图数据的邻接矩阵
def create_graph(image):
    # 图像的宽高
    height, width = image.shape[1:]
    # num_nodes = height * width
    # 创建图的边索引
    row, col = [], []
    for i in range(height):
        for j in range(width):
            if i < height - 1:
                # 上下
                row.append(i * width + j)
                col.append((i + 1) * width + j)
            if j < width - 1:
                # 左右
                row.append(i * width + j)
                col.append(i * width + (j + 1))
            if i < height - 1 and j < width - 1:
                row.append(i * width + j)
                col.append((i + 1) * width + (j + 1))
            if i < height - 1 and j > 0:
                row.append(i * width + j)
                col.append((i + 1) * width + (j - 1))
    edge_index = torch.tensor([row, col], dtype=torch.long)
    return edge_index


# ②图格式转换
'''
输入：卷积神经网络数据集
返回：
'''


def graph_trans(dataset, image_size):
    data_list = []
    for index, image in enumerate(dataset):
        # image[0]为图像，image[1]为标签，通道数为3
        # 编码像素值和像素位置为节点属性
        # ①获取图像像素值
        # print("image的像素值：")
        # print(image[0].shape)
        # print(image[0])

        # 保存图像
        # print(image[0].shape)
        # img_write = image[0].detach().cpu().numpy()
        # print(img_write.shape)
        # cv2.imwrite(str(index) + '.jpg', img_write[0]*255)

        pixel_values = image[0].view(-1, image_size * image_size)  # 节点像素值
        # print("image被拉平后的像素值：")
        # print(pixel_values.shape)
        # print(pixel_values)
        label = image[1][0].unsqueeze(0)  # 节点标签,（三通道）仅取第一通道
        threshold = 0.1
        label[label > threshold] = 1
        label[label <= threshold] = 0
        # print("label的像素值：")
        # print(label.shape)
        # print(label)
        # break
        # ②获取像素位置编码（这里简单地将位置坐标归一化到[0, 1]范围）
        x_coord, y_coord = np.meshgrid(np.linspace(0, 1, image_size), np.linspace(0, 1, image_size))
        position_encoding = np.stack((x_coord, y_coord), axis=-1)
        position_encoding = np.rollaxis(position_encoding, 2).reshape(-1, image_size * image_size)
        position_encoding = position_encoding[[1, 0]]  # 交换了数组前两列的位置
        position_encoding = torch.tensor(position_encoding, dtype=torch.float32)
        # print("image的位置向量：")
        # print(position_encoding.shape)
        # print(position_encoding[:, :10])
        # ①+②像素值+像素位置，组成节点属性
        combined_input = torch.cat((pixel_values[0].unsqueeze(0), position_encoding), dim=0)  # 灰度图像转rgb后的第一个通道
        combined_input = combined_input.view(3, image_size, image_size)  # 灰度图像转rgb后的第一个通道
        # print(combined_input.shape)
        # print(combined_input)
        combined_input = combined_input.permute(1, 2, 0).reshape(-1, 3)
        # print(combined_input)

        # 边索引
        edge_index = create_graph(image[0])
        # print("image的边向量：")
        # print(edge_index.shape)
        # print(edge_index)

        data = Data(x=combined_input, y=label.permute(1, 2, 0).reshape(-1, 1), edge_index=edge_index)  # 灰度图像转rgb后的第一个通道
        data_list.append(data)
    return data_list


def data_loader(image_size, path, symbol="infer"):
    transform = transforms.Compose([
        transforms.Resize((image_size, image_size)),  # 调整图像大小
        transforms.ToTensor()])

    if symbol == "infer":
        conv_test_dataset = ConvDataset_inf(root_dir=path,
                                            transform=transform)
        graph_test_dataset = graph_trans(conv_test_dataset, image_size)
        return conv_test_dataset, graph_test_dataset

    else:
        # 训练与测试阶段
        train_dir = path + r"\train"
        test_dir = path + r"\test"
        # 数据转换和加载
        conv_train_dataset = ConvDataset(root_dir=train_dir,
                                         transform=transform)
        conv_test_dataset = ConvDataset(root_dir=test_dir,
                                        transform=transform)

        graph_train_dataset = graph_trans(conv_train_dataset, image_size)
        graph_test_dataset = graph_trans(conv_test_dataset, image_size)
        return conv_train_dataset, conv_test_dataset, graph_train_dataset, graph_test_dataset
    # 卷积训练集 卷积测试集 图训练集 图测试集
