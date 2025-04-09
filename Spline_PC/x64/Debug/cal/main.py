from .scan.Data_Collection import scan_us
from .src.enter import enter_main, us_probe, us_phantom
from .src.cut import cut_img
import numpy as np


def main_self(scan_need, cut_need, phantom_cal_need, points_pair_need, probe_cal_need, probe, order, dir):
    if scan_need:
        # 标定数据的采集==========================================
        # us_probe为真时进行超声探头标定数据采集，为假时进行超声体模标定数据采集
        # 探头标定数据：位姿data/ndi_read{}.txt; 图像imgs/raw{};
        # 体模标定数据：位姿data/points_in_phantom.txt;
        if probe:
            print("超声探头标定数据采集")
        else:
            print("超声体模标定数据采集")
        scan_us(probe, dir)

    if cut_need:
        cut_img(dir, symbol=1, alpha=2, beta=50)

    filename_phantom = dir + r"\data\points_in_phantom.txt"
    if phantom_cal_need:
        print("体模标定0")
        # 超声体模标定============================================
        # 光学相机下的探针位姿、体模位姿
        # ①探针采集体模上标号为5-15的点，同时记录探针位姿和体模位姿
        # ②体模模型坐标系下的标记点和N线端点坐标，可通过建模得到（建模软件中点选）
        # 体模标定，旨在确定体模模型坐标系到体模坐标架坐标系间的转换，进而确定N线端点在体模坐标架坐标系下的位置
        # 将其记为points_in_phantom.txt（保存至../wts）
        # filename_phantom = r"data\points_in_phantom.txt"
        # 可多次运行，直至可靠标定误差
        # 多次运行时，之前的结果会被覆盖
        res = us_phantom(dir, filename_phantom)
        return res

    file_order = order
    # 标定图像的有效区域
    cut_file = dir + '/wts/cut_index.txt'
    # 体模坐标系下的端点坐标
    end_pose = dir + '/wts/p_end_phantom.txt'
    # 分割模型的权重文件
    model_wts = dir + '/wts/model_10.pth'
    if points_pair_need:
        # 超声探头标定============================================
        # 超声设备下的标定图像、光学相机下的标定位姿
        # # image_path = 'data/ndi_read{}.txt'.format(file_order)
        # # pose_path= 'imgs/raw{}'.format(file_order)
        # file_order = order
        # # 标定图像的有效区域
        # cut_index = np.loadtxt(r".\wts\cut_index.txt", delimiter=',').astype(int)
        # # 体模坐标系下的端点坐标
        # end_pose = 'wts/p_end_phantom.txt'
        # # 分割模型的权重文件
        # model_wts = 'wts/model_10.pth'
        cut_index = np.loadtxt(cut_file, delimiter=',').astype(int)
        enter_main(file_order, cut_index, end_pose, model_wts, dir)

    if probe_cal_need:
        # 标定矩阵记为"imgs/matrix{}.txt"
        with open(dir + r"\data\us_points_C{}.txt".format(file_order), 'r', encoding='utf-8') as file:
            lines = file.readlines()
        batch_size = len(lines)
        res = us_probe(file_order, end_pose, batch_size, dir)
        return res


def main(mode, order=0, dir="."):
    # 完整标定流程=====================
    # ①scan_need为True，采集体模标定数据，记为points_in_phantom.txt
    # ②（可重复）phantom_cal_need为True，计算N线端点坐标，记为p_end_phantom.txt
    # ③scan_need、probe为True，采集探头标定数据，记为rawN和ndi_readN.txt【采重建数据亦是】
    # ④cut_need（仅一次，cut_index.txt）选择成像区域
    # ⑤points_pair_need、probe_cal_need为True，计算超声标定矩阵，记为matrixN.txt
    # ⑥（可重复）probe_cal_need为True，计算超声标定矩阵

    # 采集标定数据
    scan_need = False
    probe = False
    # 获取图像有效范围
    cut_need = False
    # 体模标定
    phantom_cal_need = False
    # 超声标定（获取目标点集）
    points_pair_need = False
    # 超声标定（求解标定矩阵）
    probe_cal_need = False

    # ①采集体模标定数据
    if mode == 1:
        scan_need = True
    # ②计算N线端点坐标
    if mode == 2:
        phantom_cal_need = True
    # ③采集探头标定数据
    if mode == 3:
        scan_need = True
        probe = True
    # ④选择成像区域
    if mode == 4:
        cut_need = True
    # ⑤计算超声标定矩阵
    if mode == 5:
        points_pair_need = True
        probe_cal_need = True
    # ⑥再次计算矩阵
    if mode == 6:
        probe_cal_need = True

    # 超声标定（选择标定数据）
    res = main_self(scan_need, cut_need, phantom_cal_need, points_pair_need, probe_cal_need, probe, order, dir)
    if res:
        return str(res)


if __name__ == "__main__":
    main()
