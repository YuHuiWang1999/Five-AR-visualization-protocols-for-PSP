import networkx as nx
from tqdm import tqdm
import matplotlib.pyplot as plt
import os
import cv2
from torch_geometric.loader import DataLoader
from skimage.morphology import dilation, square
import numpy as np
from sympy import Matrix

# ①打印一个图结构（与神经网络的输入图结构同尺寸）==============================================================================
def node_plot(graph_train_dataset):
    # 打印与可视化
    for graph in graph_train_dataset:
        print("Node features (image):", graph.x.shape)  # 打印节点特征（图像）的形状
        print("Node features (label):", graph.y.shape)  # 打印节点特征（标签）的形状
        print("Edge index:", graph.edge_index.shape)  # 打印边的索引的形状

        # 创建图对象
        G = nx.Graph()
        # 添加节点
        num_nodes = graph.x.shape[0]
        for i in tqdm(range(num_nodes)):
            G.add_node(i)
        # 添加边
        for edge in tqdm(graph.edge_index.t().tolist()):
            G.add_edge(edge[0], edge[1])
        # 绘制图
        plt.figure(figsize=(8, 6))
        pos = nx.spring_layout(G)
        nx.draw(G, pos, node_size=10, with_labels=False, arrows=False)
        plt.title('Visualization of Graph Data')
        plt.show()
        break


# ②可视化原图、分割结果、真值标签===========================================================================================
def res_plot(batch_size, image_size, graph_test_dataset, conv_test_dataset, output_valid, output_dir, points_res):
    loader_graph = DataLoader(graph_test_dataset, batch_size=batch_size, shuffle=False)

    ensure_empty_directory(output_dir)
    for batch in loader_graph:
        # print(batch.x.shape)
        right = batch.x.shape[0] // batch_size
        # print(right)
        # break
        for index in range(len(batch)):
            # 创建画布
            plt.figure(figsize=(10, 10))

            # plt.subplot(1, 5, 1)
            # plt.imshow(conv_test_dataset[index][1].numpy().transpose(1, 2, 0), cmap='gray')
            # plt.axis('off')  # 标签（卷积）

            plt.subplot(1, 3, 1)
            plt.imshow(conv_test_dataset[index][0].numpy().transpose(1, 2, 0), cmap='gray')
            plt.axis('off')  # 原图（卷积）

            plt.subplot(1, 3, 2)
            plt.imshow(output_valid[index][0].detach().cpu().numpy(), cmap='gray')
            plt.axis('off')  # 分割结果

            save_path = os.path.join(output_dir, "output{}.jpg".format(index))
            # print(save_path)
            cv2.imwrite(save_path, output_valid[index][0].detach().cpu().numpy() * 255)

            # plt.subplot(1, 5, 4)
            # plt.imshow(batch.x[right * index:right * (index + 1)].T.view(3, image_size,
            #                                                              image_size).detach().cpu().numpy().transpose(
            #     1, 2, 0)[:, :, 0], cmap='gray')
            # plt.axis('off')  # 原图（图）

            plt.subplot(1, 3, 3)
            plt.imshow(
                batch.y[right * index:right * (index + 1)].T.view(1, image_size,
                                                                  image_size).detach().cpu().numpy().transpose(1, 2,
                                                                                                               0),
                cmap='gray')
            plt.axis('off')  # 标签（图）

            plt.show()
        break


import shutil
def ensure_empty_directory(path):
    """
    确保指定的目录存在且为空，如果不存在则创建。
    如果存在，则先清空该目录。

    :param path: 要检查或创建的目录路径
    """
    # 检查目录是否存在
    if os.path.exists(path):
        # 如果存在，则清空该目录
        shutil.rmtree(path)
        # print(f"目录 '{path}' 已被清空。")
    # 无论之前是否存在，现在都确保目录创建
    os.makedirs(path)
    # print(f"目录 '{path}' 已创建/确保为空。")


def point_in_img(ponits, image):
    if ponits is None:
        plt.figure()
        plt.imshow(image, cmap='gray')
        plt.title('image')
        plt.axis('off')  # 关闭坐标轴
        plt.show()
        return
    image = image[:, :, 0]
    image = np.stack([image, image, image], axis=-1)
    p1, p2, p3 = ponits[0], ponits[1], ponits[2]
    plt.figure()
    for i in range(len(p1)):
        for j in range(len(p1[0])):
            # print(i,j)
            # print(p1[i][j][0], p1[i][j][1])
            image[p1[i][j][0], p1[i][j][1]] = [255, 255, 0]
            image[p2[i][j][0], p2[i][j][1]] = [0, 255, 0]
            image[p3[i][j][0], p3[i][j][1]] = [255, 0, 0]
            # print([p1[i][j][0], p1[i][j][1]], [p2[i][j][0], p2[i][j][1]], [p3[i][j][0], p3[i][j][1]])
            # x_circle, y_circle = circle_center_radius([p1[i][j][0], p1[i][j][1]], [p2[i][j][0], p2[i][j][1]], [p3[i][j][0], p3[i][j][1]])
            # plt.plot(x_circle, y_circle)
            # plt.plot([p1[i][j][0], p1[i][j][1]], [p2[i][j][0], p2[i][j][1]], [p3[i][j][0], p3[i][j][1]])

    plt.imshow(image, cmap='gray')
    plt.title('image')
    plt.axis('off')  # 关闭坐标轴
    plt.show()


def circle_center_radius(p1, p2, p3):
    # Convert points to vectors
    v1 = Matrix([p2[0] - p1[0], p2[1] - p1[1]])
    v2 = Matrix([p3[0] - p1[0], p3[1] - p1[1]])

    # Calculate the determinant (cross product in 2D is essentially the determinant of the matrix formed by the vectors)
    det = v1[0] * v2[1] - v1[1] * v2[0]

    # Check if points are collinear (if determinant is zero, there's no unique circle through these points)
    if det == 0:
        if p1[0] == p2[0] and p1[0] == p3[0]:
            radius = (max(p1[1], p2[1], p3[1]) - min(p1[1], p2[1], p3[1])) / 2
            center = p1[0], max(p1[1], p2[1], p3[1]) - radius
        else:
            radius = (max(p1[0], p2[0], p3[0]) - min(p1[0], p2[0], p3[0])) / 2
            center = max(p1[1], p2[1], p3[1]) - radius, p1[1]

        # raise ValueError("The three points are collinear and do not define a unique circle.")
    else:
        # Calculate the center of the circle using the determinants
        c_x = ((v1[0] ** 2 + v1[1] ** 2) * v2[1] - (v2[0] ** 2 + v2[1] ** 2) * v1[1]) / (2 * det)
        c_y = ((v2[0] ** 2 + v2[1] ** 2) * v1[0] - (v1[0] ** 2 + v1[1] ** 2) * v2[0]) / (2 * det)
        center = (c_x + p1[0], c_y + p1[1])

        # Calculate the radius of the circle as the distance from the center to any of the three points
        radius = ((center[0] - p1[0]) ** 2 + (center[1] - p1[1]) ** 2) ** 0.5

    # Define the circle
    theta = np.linspace(0, 2 * np.pi, 100)
    x_circle = center[0] + radius * np.cos(theta)
    y_circle = center[1] + radius * np.sin(theta)

    return x_circle, y_circle
