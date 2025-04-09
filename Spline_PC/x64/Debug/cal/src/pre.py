import matplotlib.pyplot as plt
import numpy as np
from . import estimate
from . import read as Loader


def get_ends_pos(dir,filename):
    # res = Loader.ndi_read(r'C:\Users\11919\Desktop\CalGnn_0506\cal\probe_s.txt')# 完整体模下的标记点位置
    res = Loader.ndi_read(filename)  # 完整体模下的标记点位置
    # "./MRS_736760.rom",# 体模
    # "./sx_probe.rom",  # 探针
    # 体模位姿/探针位姿=================================================================================
    T_phantom2worlds = res[1]
    T_probe2worlds = res[0]

    # 体模标记点坐标（世界坐标系）========================================================================
    p_mark_world = T_probe2worlds[:, :, 3]
    # print(type(p_mark_world))

    # 体模标记点坐标（体模坐标架坐标系）===================================================================
    p_mark_phantom = np.zeros_like(p_mark_world)
    for i in range(p_mark_world.shape[0]):
        T_worlds2phantom = np.linalg.inv(T_phantom2worlds[i])
        p_mark_phantom[i] = T_worlds2phantom @ p_mark_world[i].T
    # p_mark_phantom.shape

    # 体模标记点坐标（体模模型坐标系）=====================================================================
    # point5-point15
    p_mark_model = np.array([
        [105.47, -5.25, -5, 1],
        [105.47, -5.25, 45, 1],
        [105.47, 14.75, -5, 1],
        [105.47, 19.75, 45, 1],
        [-3.53, -9.25, 45, 1],
        [41.47, -9.25, 45, 1],
        [86.47, -9.25, 45, 1],
        [96.47, -9.25, 30, 1],
        [86.47, -9.25, -5, 1],
        [21.47, -9.25, -5, 1],
        [-3.53, -9.25, -5, 1]])
    # print(p_mark_model.shape)

    # 体模的模型坐标系到坐标架坐标系=====================================================================

    points_num = p_mark_model.shape[0]
    T_model2phantom = np.zeros((4, 4))
    print("体模标定1")
    parameter = ["checking", "fixed"]
    T_model2phantom,error = estimate.matrix_cal(5, p_mark_model, p_mark_phantom, parameter, order="_phantom", dir=dir, no_scaling=True)
    print("体模标定2")
    # 体模N线端点坐标（体模模型坐标系）=====================================================================
    # K-H-E-k-h-e
    p_end_model = np.array([
        [50, 0, -40, 1],
        [35, 0, -40, 1],
        [20, 0, -40, 1],
        [50, 0, 80, 1],
        [35, 0, 80, 1],
        [20, 0, 80, 1],

        [50, 10, -40, 1],
        [35, 10, -40, 1],
        [20, 10, -40, 1],
        [50, 10, 80, 1],
        [35, 10, 80, 1],
        [20, 10, 80, 1],

        [50, 20, -40, 1],
        [35, 20, -40, 1],
        [20, 20, -40, 1],
        [50, 20, 80, 1],
        [35, 20, 80, 1],
        [20, 20, 80, 1]])
    # p_end_model

    # 体模标记点坐标（体模坐标架坐标系）=====================================================================
    p_end_phantom = np.zeros_like(p_end_model)
    for i in range(p_end_model.shape[0]):
        p_end_phantom[i] = T_model2phantom @ p_end_model[i].T
    # p_end_phantom

    # 打开一个文件用于写入
    with open(dir + '/wts/p_end_phantom.txt', 'w') as f:
        # 遍历数组的每一行
        for row in p_end_phantom:
            # 将数组的一行转换为字符串，并使用空格分隔元素
            row_str = ' '.join(map(str, row))
            # 写入文件，并在末尾添加换行符
            f.write(row_str + '\n')

    with open(dir + '/wts/T_model2phantom.txt', 'w') as f:
        # 遍历数组的每一行
        for row in T_model2phantom:
            # 将数组的一行转换为字符串，并使用空格分隔元素
            row_str = ' '.join(map(str, row))
            # 写入文件，并在末尾添加换行符
            f.write(row_str + '\n')
    print("体模标定3")
    # 创建一个新的figure，并添加一个3D的子图
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    # plt.rcParams['font.sans-serif'] = ['SimHei']
    # ax.set_title("error:", error)

    # 定义一些三维点
    points = p_end_phantom

    # 绘制这些点
    ax.scatter(points[:, 0], points[:, 1], points[:, 2])

    # 设置坐标轴标签
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    # 显示图形
    plt.show()
    print("体模标定0115")
    return error


# get_ends_pos(r"C:\Users\11919\Desktop\Cal_Project\data\points_in_phantom.txt")
