import numpy as np
import open3d as o3d
# pip install open3d
def stl_read(filename='./test/us_points.txt'):
    # 初始化一个空列表来存储读取的行
    lines = []

    # 读取文件
    with open(filename, 'r') as f:
        for line in f:
            # 去除行尾的换行符和可能的前导/尾随空格
            line = line.strip()
            # 如果行不是空的，则添加到列表中
            if line:
                lines.append(line.split())  # 使用split()按空格分割字符串为列表

    length = len(lines)
    # 将字符串列表转换为列表（或浮点数列表，如果需要）
    numeric_lists = [[float(num) for num in line] for line in lines]
    # 将二维列表转换为NumPy数组
    stl_points_3d = np.array(numeric_lists)[:,:3]
    # print(stl_points_3d.shape)
    # print(stl_points_3d[:10])
    return stl_points_3d

# 读取txt文件中的点云数据
def read_point_cloud_from_txt(file_path):
    points = []
    with open(file_path, 'r') as file:
        for line in file:
            x, y, z = map(float, line.split())
            points.append([x, y, z])
    return np.array(points)

def cloud_dis(points1, points2):

    # 将点云转换为numpy数组以便进行计算
    points_A = read_point_cloud_from_txt('path_to_pointcloud_A.txt')
    points_B = read_point_cloud_from_txt('path_to_pointcloud_B.txt')

    # 将点云数据转换为Open3D的点云对象
    pcd_A = o3d.geometry.PointCloud()
    pcd_A.points = o3d.utility.Vector3dVector(points_A)

    pcd_B = o3d.geometry.PointCloud()
    pcd_B.points = o3d.utility.Vector3dVector(points_B)

    # 初始化一个数组来存储距离
    distances = np.zeros(points_B.shape[0])

    # 对于点云B中的每个点，计算它到点云A中最近点的距离
    for i, point_B in enumerate(points_B):
        # 使用Open3D的KDTree来搜索最近点
        tree = o3d.geometry.KDTreeFlann(pcd_A)
        [_, idx, dist] = tree.search_knn_vector_3d(point_B, 1)  # K=1表示只搜索最近的一个点
        # 由于我们只需要最近的一个点，所以取dist数组的第一个元素
        distances[i] = dist[0]

    # 打印距离或进行其他处理
    print("Distances from each point in B to the nearest point in A:")
    print(distances)

    # 可视化结果（可选）
    # 例如，可以将距离作为颜色编码绘制到点云B上
    colors = [[distances[i] / np.max(distances), 0, 1 - distances[i] / np.max(distances)] for i in
              range(len(distances))]
    pcd_B.colors = o3d.utility.Vector3dVector(colors)
    o3d.visualization.draw_geometries([pcd_B])  # 只绘制点云B以查看颜色编码的距离

if __name__ == '__main__':
    stl_read()