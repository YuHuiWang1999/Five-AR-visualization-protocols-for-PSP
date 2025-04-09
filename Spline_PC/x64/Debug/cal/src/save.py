import numpy as np
import cv2
import os
from . import read as Loader

# from save import ensure_dir

# ①裁剪并保存图片========================================================================================================
def save_pic(src_folder, order, cut_index,dir):
    # 指定原始图片文件夹和裁剪后图片保存文件夹
    dst_folder = (dir+'/imgs/cut{}/imgs'.format(order))
    ensure_dir(dst_folder)
    # 如果保存文件夹不存在，则创建它
    if not os.path.exists(dst_folder):
        os.makedirs(dst_folder)
    # 遍历原始图片文件夹中的所有文件
    # index = 1
    listdir = os.listdir(dir+'/imgs/raw{}/imgs'.format(order))
    # print(listdir)
    error_index = Loader.ndi_readerror(dir+r'/data/ndi_read{}.txt'.format(order))
    for index, filename in enumerate(listdir):
        if index in error_index:
            continue
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            # print(index, filename)
            # index = index + 1
            # 读取图片
            img = cv2.imread(os.path.join(src_folder, filename))
            x1, y1, x2, y2 = cut_index[0], cut_index[1], cut_index[2], cut_index[3]
            # 裁剪图片
            cropped_img = img[y1:y2, x1:x2]
            # 保存裁剪后的图片
            cv2.imwrite(os.path.join(dst_folder, filename), cropped_img)
    print("裁剪并保存图片完成。")


def ensure_dir(file_path):
    """
    确保文件所在的目录存在，如果不存在，则创建该目录。

    参数:
    file_path (str): 文件的完整路径
    """
    directory = os.path.dirname(file_path)
    if not os.path.exists(directory):
        os.makedirs(directory)


def line_numbers_to_keep(folder_path, error):
    # 指定文件夹路径
    name_list = []
    # 遍历文件夹
    for filename in os.listdir(folder_path):
        name, extension = os.path.splitext(filename)
        # 只打印不包含后缀的文件名
        # print(name)
        name_list.append(int(name))
    name_list.sort()
    for e in error:
        name_list.remove(e)
    return name_list, len(name_list)


def keep_specific_lines(good_index, txt_paths):
    """
    读取txt文件，并仅保留指定的行号。

    :param file_path: str, 要读取并修改的文件路径
    :param line_numbers_to_keep: list of int, 要保留的行号列表，注意行号从1开始
    :return: None，直接修改文件
    """
    # 要保留的行
    lines_to_keep = good_index
    lines_to_keep.sort()
    # 读取文件内容到列表
    for path in txt_paths:
        if path is None:
            continue
        with open(path, 'r', encoding='utf-8') as file:
            lines = file.readlines()
            # 根据索引保留指定的行
        new_lines = [lines[index].strip() + '\n' for index in lines_to_keep if index < len(lines)]
        # 将保留的行写回文件
        with open(path, 'w', encoding='utf-8') as file:
            file.writelines(new_lines)

import re
def ndi_del(file_name,del_index):
    # 读取txt文件
    print("ndi_del")
    with open(file_name, 'r', encoding='utf-8') as file:
        lines = file.readlines()
    print("ndi_del")
    result = [line.strip() + '\n' for line in lines]
    # # 使用正则表达式作为分割符，以数字为分隔符
    # result = re.split("\n", data)
    # result.pop()

    error_index = []
    for i, index in enumerate(result):
        if 'nan' in index:
            # error_index.append(i + 1)
            error_index.append(i)
            continue

    for i in error_index[::-1]:
        result.pop(i)

    for i in del_index[::-1]:
        result.pop(i)
    print("ndi_del")
    with open(file_name, 'w', encoding='utf-8') as file:
        file.writelines(result)


import math


def calculate_distances(A, B):
    """
    计算两个二维点集A和B对应位置的距离误差
    :param A: 第一个点集，形如[(x1, y1), (x2, y2), ...]
    :param B: 第二个点集，形如[(x1, y1), (x2, y2), ...]
    :return: 距离误差列表
    """
    distances = []
    for a, b in zip(A, B):
        dx = a[0] - b[0]
        dy = a[1] - b[1]
        distance = math.sqrt(dx ** 2 + dy ** 2)
        distances.append(distance)
    return distances


from .read import us_read as us_read
import numpy as np

if __name__ == '__main__':
    # 不要删除======
    # # 计算提取点与标签点的距离误差，现在是在筛选有效的对应点集test_sort.注意提前备份未筛选的原点集test
    # for symbol in ["train", "test"]:
    #     # symbol = "train"
    #     pic_path = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test\output_C{}_sort'.format(symbol)
    #     txt_path1 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test\label_C{}.txt'.format(
    #         symbol)  # 替换为你的文件路径
    #     txt_path2 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test\output_C{}.txt'.format(symbol)
    #     txt_path3 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test\label_P{}.txt'.format(
    #         symbol)  # 替换为你的文件路径
    #     txt_path4 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test\output_P{}.txt'.format(symbol)
    #     keep_specific_lines(pic_path, [txt_path1, txt_path2, txt_path3, txt_path4])

    symbol = "train"
    txt_path1 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test_sort\label_C{}.txt'.format(symbol)
    train_label_C, _, _ = us_read(txt_path1)
    print("train_label_C", train_label_C.shape)
    txt_path2 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test_sort\output_C{}.txt'.format(symbol)
    train_output_C, _, _ = us_read(txt_path2)
    print("train_output_C", train_output_C.shape)
    txt_path3 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test_sort\label_P{}.txt'.format(symbol)
    train_label_P, _, _ = us_read(txt_path3)
    print("train_label_P", train_label_P.shape)
    txt_path4 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test_sort\output_P{}.txt'.format(symbol)
    train_output_P, _, _ = us_read(txt_path4)
    print("train_output_P", train_output_P.shape)

    symbol = "test"
    txt_path1 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test_sort\label_C{}.txt'.format(symbol)
    test_label_C, _, _ = us_read(txt_path1)
    print("test_label_C", test_label_C.shape)
    txt_path2 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test_sort\output_C{}.txt'.format(symbol)
    test_output_C, _, _ = us_read(txt_path2)
    print("test_output_C", test_output_C.shape)
    txt_path3 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test_sort\label_P{}.txt'.format(symbol)
    test_label_P, _, _ = us_read(txt_path3)
    print("test_label_P", test_label_P.shape)
    txt_path4 = r'C:\Users\11919\Desktop\CalGnn_0506\Cal_auto2024\data\test_sort\output_P{}.txt'.format(symbol)
    test_output_P, _, _ = us_read(txt_path4)
    print("test_output_P", test_output_P.shape)

    # P是预测结果。C是校正结果
    distance_train_P = calculate_distances(train_label_P[:, :, :, :2].reshape(-1, 2),
                                           train_output_P[:, :, :, :2].reshape(-1, 2))
    distance_train_P = np.array(distance_train_P)
    print("distance_train_P", distance_train_P.shape, np.max(distance_train_P), np.min(distance_train_P),
          np.mean(distance_train_P))

    distance_train_C = calculate_distances(train_label_C[:, :, :, :2].reshape(-1, 2),
                                           train_output_C[:, :, :, :2].reshape(-1, 2))
    distance_train_C = np.array(distance_train_C)
    print("distance_train_C", distance_train_C.shape, np.max(distance_train_C), np.min(distance_train_C),
          np.mean(distance_train_C))

    distance_test_P = calculate_distances(test_label_P[:, :, :, :2].reshape(-1, 2),
                                          test_output_P[:, :, :, :2].reshape(-1, 2))
    distance_test_P = np.array(distance_test_P)
    print("distance_test_P", distance_test_P.shape, np.max(distance_test_P), np.min(distance_test_P),
          np.mean(distance_test_P))

    distance_test_C = calculate_distances(test_label_C[:, :, :, :2].reshape(-1, 2),
                                          test_output_C[:, :, :, :2].reshape(-1, 2))
    distance_test_C = np.array(distance_test_C)
    print("distance_test_C", distance_test_C.shape, np.max(distance_test_C), np.min(distance_test_C),
          np.mean(distance_test_C))

    # distance_train_P (1905,) 3.1622776601683795 0.0 0.6291121523113569
    # distance_train_C (1905,) 3.1622776601683795 0.0 0.7197441174677733
    # distance_test_P (180,) 3.1622776601683795 0.0 0.6919251431565463
    # distance_test_C (180,) 3.1622776601683795 0.0 0.7554621109159768
