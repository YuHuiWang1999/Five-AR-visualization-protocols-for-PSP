import numpy as np
import re



# ①读取本地的txt文件，文件中包含从超声图像中提取的标定点坐标，形状为(batch_size,3,5,4)===========================================
# 输入：从图像内提取的二维点坐标； 输出：完整点集的齐次坐标/中间（不包含左右端点）的齐次坐标/点集距离关系
def us_read(filename='./test/us_points.txt', n_Phantom=3, batch_size=18):
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
    # 将字符串列表转换为整数列表（或浮点数列表，如果需要）
    numeric_lists = [[float(num) for num in line] for line in lines]

    # 将二维列表转换为NumPy数组
    us_points_2d = np.array(numeric_lists)
    us_points_2d = us_points_2d.reshape(length, n_Phantom, 5, 2)
    zeros_z = np.zeros((length, n_Phantom, 5, 1), dtype=us_points_2d.dtype)  # z轴坐标0
    ones_w = np.ones((length, n_Phantom, 5, 1), dtype=us_points_2d.dtype)  # 齐次坐标1
    # 使用 np.concatenate 或者 np.dstack (对于深度堆叠)
    us_points_4d_5 = np.concatenate((us_points_2d, zeros_z, ones_w), axis=-1)  # 使用 concatenate
    us_points_4d_3 = us_points_4d_5[:, :, 1:4, :]

    # us_points_4d_5 = np.squeeze(us_points_4d_5,1)
    # us_points_4d_3 = np.squeeze(us_points_4d_3,1)

    scales = scale_cal(us_points_4d_5, length, n_Phantom)
    # print("us_points_4d_5 的形状：", us_points_4d_5.shape)
    # # print(us_points_4d_5)
    # print("us_points_4d_3 的形状：", us_points_4d_3.shape)
    # print(us_points_4d_3)
    # print("scales 的形状：", scales.shape)
    # # print(scales)
    return us_points_4d_5, us_points_4d_3, scales


def scale_cal(us_points, batch_size=18, n_Phantom=3):
    # 计算每张图像中各点之间的距离
    distance = np.zeros((batch_size, n_Phantom, 5, 5))
    for item, image in enumerate(us_points):
        for index, pixls in enumerate(image):
            # 使用NumPy的广播功能计算所有点之间的距离
            # 计算每对点之间的差值
            # print("pixls",pixls)
            point_diff = pixls[:, np.newaxis, :] - pixls[np.newaxis, :, :]

            # 计算差值的平方
            point_diff_squared = point_diff ** 2

            # 沿着坐标轴相加
            distance_squared = np.sum(point_diff_squared, axis=2)

            # 对距离的平方进行开方，得到距离
            distance[item][index] = np.sqrt(distance_squared)
    # print("distance",distance)
    scale = np.zeros((batch_size, n_Phantom, 3))

    for item, image in enumerate(distance):
        for index, d in enumerate(image):
            # print(distance.shape, image.shape)
            # print(item,index)
            scale[item][index][0], scale[item][index][1], scale[item][index][2] = d[0][1] / d[1][2], d[1][2] / d[2][3], d[2][3] / d[3][4]
    return scale


# ②从ndi中读取的 探头到ndi、体模到ndi的转换矩阵，形状均为(batch_size,4,4)=====================================================
def ndi_read(file_name):
    us_pose = []
    phantom_pose = []

    # 读取txt文件
    with open(file_name, 'r') as file:
        data = file.read()

    # 删除换行符
    data = data.replace("\n", "")
    # 使用正则表达式作为分割符，以数字为分隔符
    result = re.split(r'\d+\.\s', data)
    result.pop(0)

    # # 使用正则表达式作为分割符，以数字为分隔符
    # result = re.split("\n", data)
    # result.pop()

    error_index = []
    for i, index in enumerate(result):
        if 'nan' in index:
            # error_index.append(i + 1)
            error_index.append(i)
            continue
        index = eval(index)
        us_pose.append(index[0])
        phantom_pose.append(index[1])
    # print(error_index)

    row = len(us_pose)
    us_pose = np.array(us_pose).reshape(row, 4, 4)
    phantom_pose = np.array(phantom_pose).reshape(row, 4, 4)

    # print("the shape of us_pose:")
    # print(us_pose.shape)
    # print(us_pose)
    # print("the shape of phantom_pose:")
    # print(phantom_pose.shape)
    # print(phantom_pose)

    return us_pose, phantom_pose,error_index

def ndi_readerror(file_name):
    # 读取txt文件
    with open(file_name, 'r') as file:
        data = file.read()

    # 使用正则表达式作为分割符，以数字为分隔符
    result = re.split("\n", data)
    result.pop()

    error_index = []
    for i, index in enumerate(result):
        if 'nan' in index:
            # error_index.append(i + 1)
            error_index.append(i)
            continue
    # print("error_index",error_index)
    return error_index


# 计算体模中的特征定位置
def calculate_midpoint_3d(A, B, k):
    """
    计算三维坐标点的中间点坐标

    参数:
    A: tuple, 第一个点的坐标 (A_x, A_y, A_z)
    B: tuple, 第二个点的坐标 (B_x, B_y, B_z)
    k: float, 中间点到 A 的距离与中间点到 B 的距离之比

    返回值:
    P: tuple, 中间点的坐标 (P_x, P_y, P_z)
    """
    A_x, A_y, A_z, A_w = A
    B_x, B_y, B_z, B_w = B

    P_x = (k * B_x + A_x) / (k + 1)
    P_y = (k * B_y + A_y) / (k + 1)
    P_z = (k * B_z + A_z) / (k + 1)

    return [P_x, P_y, P_z, 1]


def phantom_point_coordinate(p_ends, scale, batch_size=18, n_Phantom=3):
    phantom_points = np.zeros((batch_size, n_Phantom, 3, 4))

    for item, image in enumerate(scale):

        for index, s in enumerate(image):
            k1, k2, k3 = s[0], s[1], s[2]
            # phantom_points[item][index][0] = calculate_midpoint_3d(p_ends[5 + index * 6], p_ends[1 + index * 6], k1)
            # phantom_points[item][index][1] = calculate_midpoint_3d(p_ends[1 + index * 6], p_ends[4 + index * 6], k2)
            # phantom_points[item][index][2] = calculate_midpoint_3d(p_ends[4 + index * 6], p_ends[0 + index * 6], k3)
            # print(item,index)
            phantom_points[item][index][0] = calculate_midpoint_3d(p_ends[0+index*6], p_ends[4+index*6], k1)
            phantom_points[item][index][1] = calculate_midpoint_3d(p_ends[4+index*6], p_ends[1+index*6], k2)
            phantom_points[item][index][2] = calculate_midpoint_3d(p_ends[1+index*6], p_ends[5+index*6], k3)

    # print("the shape of phantom__points:")
    # print(phantom_points.shape)
    # print(P_phantom_fin)
    # print("the shape of phantom_points:")
    # print(phantom_points.shape)
    return phantom_points


# ③体模中N线端点在体模坐标架坐标系下的位置坐标，形状均为(6*3,8)，(6*3,3)也许足够==================================================
def p_ends_read(txt_file, n_Phantom):
    with open(txt_file, 'r') as file:
        lines = file.readlines()
        data = []

        for index_row, line in enumerate(lines):
            line = line.strip()  # 移除每行的换行符和空格
            if line:
                # 将每行以制表符或空格分割为数字，然后转换为浮点数
                if n_Phantom == 1:
                    row = [float(num) for num in line.split('\t')]  # n_Phantom = 1时
                elif n_Phantom == 3:
                    row = [float(num) for num in line.split(' ')]
                else:
                    row = None
                data.append(row)

    # 将数据列表转换为NumPy数组
    p_ends = np.array(data)
    # print("the shape of p_ends:")
    # print(p_ends.shape)
    # print(p_ends)

    return p_ends


# ④获取配对点坐标========================================================================================================
def paired_points(us_points, phantom_points, T_probe2worlds, T_phantom2worlds, n_Phantom=3):
    line_count = us_points.shape[0]
    known_points_before = us_points.reshape(line_count * n_Phantom * 3, 4)
    # print("the shape of known_points_before:")
    # print(known_points_before.shape)
    # print(known_points_before)

    # print("the shape of P_phantom_fin:")
    # print(P_phantom_fin.shape)
    # print("the shape of T_probe2world:")
    # print(T_probe2world.shape)
    # print("the shape of T_phantom2world:")
    # print(T_phantom2world.shape)

    known_points_after = np.zeros((line_count * n_Phantom * 3, 4))
    # known_points_world = np.zeros((line_count * n_Phantom * 3, 4))
    # 每张图像
    for index_2, point_2 in enumerate(phantom_points):
        # 每张图像一组转换矩阵
        T_world2probe = np.linalg.inv(T_probe2worlds[index_2])
        T_phantom2world = T_phantom2worlds[index_2]
        # 每行n线
        for index_1, point_1 in enumerate(point_2):
            # 每个标定点
            for index_3, point_3 in enumerate(point_1):
                # print(index_2 * 9 + index_1 * 3 + index_3)
                known_points_after[
                    index_2 * 3 * n_Phantom + index_1 * 3 + index_3] = T_world2probe @ T_phantom2world @ point_3.T


    # print("the shape of known_points_after:")
    # print(known_points_after.shape)
    # print(known_points_after)

    return known_points_before, known_points_after


if __name__ == '__main__':
    res_us = ndi_read(r'C:\Users\11919\Desktop\Tju_MRS\co_hospi\Desktop\out\1.txt')  # 完整体模下的标记点位置
    # "./MRS_736760.rom",# 体模
    # "./sx_probe.rom",  # 探针
    Tus_phantom2worlds = res_us[0]
    Tus_probe2worlds = res_us[1]