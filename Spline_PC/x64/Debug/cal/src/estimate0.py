import math
from scipy.optimize import minimize
import numpy as np


# 最小二乘法求解标定矩阵
# ①定义误差函数，这里使用平方误差===========================================================================================
def error_function(parameters, known_points_before, known_points_after):
    # parameters 包含变换矩阵的参数

    angle_x, angle_y, angle_z = parameters[:3]
    # 创建旋转矩阵
    rotation_x = np.array([
        [1, 0, 0, 0],
        [0, math.cos(angle_x), math.sin(angle_x), 0],
        [0, -math.sin(angle_x), math.cos(angle_x), 0],
        [0, 0, 0, 1]
    ])
    rotation_y = np.array([
        [math.cos(angle_y), 0, math.sin(angle_y), 0],
        [0, 1, 0, 0],
        [-math.sin(angle_y), 0, math.cos(angle_y), 0],
        [0, 0, 0, 1]
    ])
    rotation_z = np.array([
        [math.cos(angle_z), -math.sin(angle_z), 0, 0],
        [math.sin(angle_z), math.cos(angle_z), 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ])

    scale_x, scale_y, scale_z = parameters[3:6]
    # 创建缩放矩阵
    scaling = np.array([
        [scale_x, 0, 0, 0],
        [0, scale_y, 0, 0],
        [0, 0, scale_z, 0],
        [0, 0, 0, 1]
    ])

    translate_x, translate_y, translate_z = parameters[6:9]
    # 创建平移矩阵
    translation = np.array([
        [1, 0, 0, translate_x],
        [0, 1, 0, translate_y],
        [0, 0, 1, translate_z],
        [0, 0, 0, 1]
    ])

    # 合并所有矩阵以获得最终的转换矩阵
    # transform_matrix = np.dot(translation, np.dot(rotation_z, np.dot(rotation_y, np.dot(rotation_x, scaling))))
    transform_matrix = translation @ rotation_z @ rotation_y @ rotation_x @ scaling

    # 应用变换矩阵到已知点
    # transformed_points = np.dot(transform_matrix, known_points_before.T)
    transformed_points = transform_matrix @ known_points_before.T
    transformed_points = transformed_points[:3].T

    # 计算平方误差
    error = (transformed_points - known_points_after[:, :3]) ** 2
    error = np.sum(error, axis=1)
    error = np.sqrt(error)
    error = np.sum(error)

    return error


def matrix_cal(num_train, known_points_before, known_points_after, parameter, n_end=False, optimism_fun="powell",
               order=9999, dir=".", no_scaling=False):
    # 初始参数猜测
    initial_parameters = np.random.rand(9)
    # 使用最小二乘法进行优化
    print("体模标定00")
    # result = minimize(error_function, initial_parameters,
    #                   args=(known_points_before[:num_train], known_points_after[:num_train]), method=optimism_fun)
    if no_scaling:
        result = minimize(error_function, initial_parameters,
                          args=(known_points_before[:num_train], known_points_after[:num_train]), method="bfgs")
    else:
        result = minimize(error_function, initial_parameters,
                          args=(known_points_before[:num_train], known_points_after[:num_train]), method=optimism_fun)

    parameters = result.x
    angle_x, angle_y, angle_z = parameters[:3]
    # 创建旋转矩阵
    rotation_x = np.array([
        [1, 0, 0, 0],
        [0, math.cos(angle_x), math.sin(angle_x), 0],
        [0, -math.sin(angle_x), math.cos(angle_x), 0],
        [0, 0, 0, 1]
    ])
    rotation_y = np.array([
        [math.cos(angle_y), 0, math.sin(angle_y), 0],
        [0, 1, 0, 0],
        [-math.sin(angle_y), 0, math.cos(angle_y), 0],
        [0, 0, 0, 1]
    ])
    rotation_z = np.array([
        [math.cos(angle_z), -math.sin(angle_z), 0, 0],
        [math.sin(angle_z), math.cos(angle_z), 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ])

    if no_scaling:
        scaling = np.array([
            [1, 0, 0, 0],
            [0, 1, 0, 0],
            [0, 0, 1, 0],
            [0, 0, 0, 1]
        ])
    else:
        scale_x, scale_y, scale_z = parameters[3:6]
        # 创建缩放矩阵
        scaling = np.array([
            [scale_x, 0, 0, 0],
            [0, scale_y, 0, 0],
            [0, 0, scale_z, 0],
            [0, 0, 0, 1]
        ])
    print("体模标定01")
    # print("angle_x={}, angle_y={}, angle_z={}".format(angle_x, angle_y, angle_z))
    #
    # print("the matrix of scaling:")
    # print(scaling)
    np.savetxt(dir + r"/imgs/scaling{}.txt".format(order), scaling, fmt='%f', delimiter=',')

    translate_x, translate_y, translate_z = parameters[6:9]
    # 创建平移矩阵
    translation = np.array([
        [1, 0, 0, translate_x],
        [0, 1, 0, translate_y],
        [0, 0, 1, translate_z],
        [0, 0, 0, 1]
    ])

    # 标定矩阵***********************************************************************************************************
    estimated_transform_matrix = translation @ rotation_z @ rotation_y @ rotation_x @ scaling

    # if n_end:
    #     return estimated_transform_matrix
    # 标定矩阵***********************************************************************************************************
    np.savetxt(dir + r"/imgs/matrix{}.txt".format(order), estimated_transform_matrix, fmt='%f', delimiter=',')

    # print("the matrix of estimated_transform_matrix:")
    # print(estimated_transform_matrix)

    matrix_no_scaling = translation @ rotation_z @ rotation_y @ rotation_x
    np.savetxt(dir + r"/imgs/matrix_no_scaling{}.txt".format(order), matrix_no_scaling, fmt='%f', delimiter=',')
    # print("the matrix of estimated_no_scaling:")
    # print(estimated_no_scaling)

    total_point_nums = known_points_before.shape[0]
    # 验证点
    if parameter[0] == "checking":
        if parameter[1] == "fixed":
            # 验证点数目指定，验证指定数目下的标定精度
            error = error_evaluate(estimated_transform_matrix, known_points_before[num_train:],
                                   known_points_after[num_train:])
            print(error / (total_point_nums - num_train))

            # error = error_evaluate(estimated_transform_matrix, known_points_before[:num_train],
            #                        known_points_after[:num_train])
            # print(error / num_train)

        elif parameter[1] == "variable":
            # 验证点数目固定，验证30个点下的标定精度
            error = error_evaluate(estimated_transform_matrix, known_points_before[total_point_nums - 45:],
                                   known_points_after[total_point_nums - 45:])
            print(error / 45)
        else:
            pass

    elif parameter[0] == "iteration":
        # 验证所有迭代点下的标定误差
        error = error_evaluate(estimated_transform_matrix, known_points_before[:num_train],
                               known_points_after[:num_train])
        print("验证所有迭代点下的标定误差")
        print(error / num_train)
        print("======================================")
        # # 验证每个迭代点下的标定误差
        # for i in range(0, num_train):
        #     error = error_evaluate(estimated_transform_matrix, known_points_before[i:i + 1],
        #                            known_points_after[i:i + 1])
        #     print(error)
    else:
        print("error!!!")
    print("体模标定02")
    # np.savetxt(dir + r"/imgs/error{}.txt".format(order), error/(total_point_nums - num_train), fmt='%f', delimiter=',')
    print("体模标定03")
    return estimated_transform_matrix, error / (total_point_nums - num_train)


def error_evaluate(transform_matrix, known_points_before, known_points_after):
    # 应用变换矩阵到已知点
    # transformed_points = np.dot(transform_matrix, known_points_before.T)
    transformed_points = transform_matrix @ known_points_before.T
    transformed_points = transformed_points[:3].T

    # 完全误差
    error = np.sum((transformed_points - known_points_after[:, :3]) ** 2, axis=1)
    error = np.sqrt(error)
    # print(".......................................")
    # print(known_points_before)
    # print(transformed_points)
    # print(known_points_after[:, :3])

    return np.sum(error)
