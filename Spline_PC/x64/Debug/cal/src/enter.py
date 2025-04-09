from .model import Mymodel as Mymodel
from .predict import model_valid, model_train, model_test
from . import read as Loader
from . import loader
from .plot import res_plot
from . import estimate
from .save import save_pic, ensure_dir
import cv2
import os
import numpy as np
import shutil

def segmented(model_wts, order, batch_size, fold, symbol="infer", cut_index=[155, 122, 433, 341], image_size=128,dir="."):
    def count_files_in_folder(folder_path):
        """
        计算指定文件夹下的文件数量（不包括子文件夹中的文件）。

        :param folder_path: 文件夹的路径
        :return: 文件夹下的文件数量
        """
        count = 0
        # 遍历文件夹
        for item in os.listdir(folder_path):
            # 构造完整路径
            full_path = os.path.join(folder_path, item)
            # 检查是否是文件（不是目录）
            if os.path.isfile(full_path):
                count += 1
        return count
    image_size = 128  # 图像尺寸
    batch_size = batch_size  # 神经网络训练时的batch_size为18，改为其他大小可以推理，但是精度下降
    # weight = r'../wts/gccnmodel_shaped06weights.pth'  # 神经网络的训练权重位置,batchsize=18
    # weight = r'../wts/model_{}.pth'.format(batch_size)  # 神经网络的训练权重位置,batchsize=4
    weight = model_wts
    model = Mymodel  # 神经网络模型
    print("=====分割模型初始化=====")

    symbol = symbol
    if symbol == "infer":
        # ①原始采集数据集->裁剪原采集图像内的成像区域=========================================================================
        save_pic(dir + r"/imgs/raw{}/imgs".format(order), order, cut_index,dir)
        print("=====标定图像数据裁剪=====")

        # 求解标定矩阵时==================
        # ②数据集加载====================================================================================================
        dataset_dir = dir + r"/imgs/cut{}".format(order)  # 数据集路径
        addition, epoc = count_files_in_folder("{}/imgs".format(dataset_dir)) % batch_size, count_files_in_folder(
            "{}/imgs".format(dataset_dir)) // batch_size
        # print("addition, epoc",addition, epoc)
        conv_test_dataset, graph_test_dataset = loader.data_loader(image_size, dataset_dir, symbol="infer")
        print("=====标定图像数据加载=====")

        # ③模型推理======================================================================================================
        model_valid(model, weight, batch_size, image_size, graph_test_dataset, addition,
                    epoc, order, fold,cut_index,dir)
        print("=====标定图像数据推理=====")

    elif symbol == "test":
        # 求解标定矩阵时==================
        # 数据集加载
        dataset_dir = r"../dataset"  # 数据集路径
        conv_train_dataset, conv_test_dataset, graph_train_dataset, graph_test_dataset = Loader.data_loader(
            image_size, dataset_dir, symbol="test")
        # 模型推理
        target = "test"
        if target == "train":
            model_test(model, weight, batch_size, image_size, graph_train_dataset, "train", fold)
        else:
            model_test(model, weight, batch_size, image_size, graph_test_dataset, "test", fold)
    else:
        # 训练分割模型时==================
        # batch_size = 4
        # 数据集加载
        dataset_dir = r"../dataset"  # 数据集路径
        conv_train_dataset, conv_test_dataset, graph_train_dataset, graph_test_dataset = Loader.data_loader(
            image_size, dataset_dir, symbol="train")
        # 模型训练
        model_train(model, batch_size, graph_train_dataset, graph_test_dataset, image_size, Epoch=500)

    # # 打印模型预测结果与真值
    # output_dir = "../imgs/seg"  # 校正后图像坐标系下的采集点坐标的保存位置
    # ensure_dir(output_dir)
    # print("=====自动提取结果保存=====")
    # # 打印模型预测结果与真值
    # res_plot(batch_size, image_size, graph_test_dataset, conv_test_dataset, output, output_dir, point_res)
    # print("自动提取结果可视化")
    # # 打印节点联通方式（示范）
    # node_plot(graph_train_dataset) # 修改image_size为4


def calculated(order, mode="predit", validation=0, batch_size=4, p_ends_file=r'../wts/p_end_phantom.txt',dir="."):
    n_Phantom = 3
    print("103")
    if n_Phantom == 3:
        # true_index = [0, 1, 2, 6]
        batch_size = batch_size
        # ①读取标定所需数据，共需要三个文件：ndi位姿文件，图像点提取文件，n线端点文件==============================================
        # 图像点提取文件*****************
        # 第一目标点集（图像）=============================================================
        if mode == "predit":
            _, us_points, scales = Loader.us_read(dir + r'./data/us_points_P{}.txt'.format(order), n_Phantom)
        else:
            _, us_points, scales = Loader.us_read(dir + r'./data/us_points_C{}.txt'.format(order), n_Phantom)
        # ndi位姿文件*****************
        us_pose, phantom_pose, _ = Loader.ndi_read(dir + r'./data/ndi_read{}.txt'.format(order))
        # n线端点文件*****************
        p_ends = Loader.p_ends_read(p_ends_file, n_Phantom)

    elif n_Phantom == 1:  # 弃用，原为通过单层N线体模上的测试数据验证标定数学模型的准确性
        batch_size = 15  # n_Phantom = 1时
        _, us_points, scales = Loader.us_read(r'/one/us_coordinate.txt',
                                              n_Phantom)  # n_Phantom = 1时
        us_pose, phantom_pose = Loader.ndi_read(
            r'/one/pose.txt')  # n_Phantom = 1时
        p_ends = Loader.p_ends_read(
            r'/one/P_phantom.txt', n_Phantom)  # n_Phantom = 1时
    else:
        batch_size = None
        _, us_points, scales = None, None, None
        us_pose, phantom_pose = None, None
        p_ends = None
    # print('Not enough')
    # print("batch_size", batch_size)
    # print("us_points", us_points.shape)
    # print("scales", scales.shape)
    # print("us_pose", us_pose.shape)
    # print("phantom_pose", phantom_pose.shape)
    # print("p_ends", p_ends.shape)
    # print(p_ends)
    # us_points, scales = us_points[true_index], scales[true_index]
    # print("us_points", us_points.shape)
    # print("scales", scales.shape)
    # print("us_pose", us_pose.shape)
    # print("phantom_pose", phantom_pose.shape)
    print("=====矩阵求解数据导入=====")

    # 第二目标点集（体模）=============================================================
    phantom_points = Loader.phantom_point_coordinate(p_ends, scales, us_points.shape[0], n_Phantom)
    # print("phantom_points", phantom_points.shape)
    known_points_before, known_points_after = Loader.paired_points(us_points, phantom_points, us_pose, phantom_pose,
                                                                   n_Phantom)
    # print("known_points_before====================================")
    # print(known_points_before)
    # print("known_points_after====================================")
    # print(known_points_after)

    print("=====标定点匹配=====")

    points_num = known_points_before.shape[0]
    print("points_num", points_num)

    optimism_fun = ['powell', 'cg', 'bfgs', 'l-bfgs-b', 'slsqp']
    parameter = [["checking", "fixed"],  # 验证剩余标定点下的标定精度
                 ["checking", "variable"],  # 验证后10个标定点下的标定精度
                 ["iteration", "fixed"]]  # 验证所有迭代点下的标定误差

    print("不同优化算法下的标定精密度{}=======================================".format(mode))
    # assert points_num >= 9 * 5
    if validation == 1:
        for item in range(9, points_num + 1, 9):
            for opt in optimism_fun:
                par = parameter[validation]
                # print("optimism_fun", opt)
                # print("loss_fun", par)
                print("使用{}张标定图像".format(item//9))
                matrix,error = estimate.matrix_cal(item, known_points_before, known_points_after, par, False, opt, order,dir,False)
                break
        print("=====标定矩阵求解=====")

    elif validation == 0:
        print("parameter",parameter)
        for opt in optimism_fun:
            par = parameter[validation]
            # print("optimism_fun", opt)
            # print("loss_fun", par)
            matrix,error = estimate.matrix_cal(points_num // 2, known_points_before, known_points_after, par, False, opt, order,dir=dir)
            break
        # print("=====标定矩阵求解=====")
    # print("error",error)
    return error

def enter_main(file_order, cut_index, end_pose, model_wts,dir):

    def on_mouse_click(event, x, y, flags, param):
        # 检查是否是右键点击
        if event == cv2.EVENT_RBUTTONDOWN:
            if int(param) not in selected_images:
                # 假设param是图像的文件名（不带路径和后缀）
                selected_images.append(int(param))
                print(f"Selected: {param}")
        if event == cv2.EVENT_LBUTTONDOWN:
            if int(param) in selected_images:
                selected_images.remove(int(param))
                print(f"Removed: {param}")


    # 读取文件夹中的所有图片
    def read_images_from_folder(folder_path, order):
        images = []
        for filename in os.listdir(folder_path):
            if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp', '.tiff')):
                img_path = os.path.join(folder_path, filename)
                img = cv2.imread(img_path)
                if img is not None:
                    img = cv2.resize(img, (500, 500))
                    # 显示图片时，文件名作为窗口名传递，用于鼠标回调
                    cv2.imshow(filename, img)
                    cv2.setMouseCallback(filename, on_mouse_click, filename.split('.')[0])
                    images.append(img)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        # raw_file_count = count_files_in_folder("../imgs/raw{}/imgs".format(order))
        # print("Top15:{}/{}==========================".format(len(selected_images), raw_file_count))
        return selected_images


    def del_search(list2, list1):
        dif = []
        for i in list1:
            if i not in list2:
                dif.append(i)
        # print("dif",dif)
        return dif


    def get_filenames_without_extension(directory):
        filenames = []
        for root, dirs, files in os.walk(directory):
            for file in files:
                # 使用os.path.splitext分离文件名和后缀
                base_name = os.path.splitext(file)[0]
                filenames.append(int(base_name))
        filenames.sort()
        # print(filenames)
        return filenames

    def check_and_replace_txt_file(file_path, backup_folder):
        # 获取文件名（不包括路径）
        file_name = os.path.basename(file_path)
        # 构造备份文件的完整路径
        backup_file_path = os.path.join(backup_folder, file_name)
        # 检查文件是否存在
        if os.path.isfile(file_path):
            # 删除原始文件
            os.remove(file_path)
            # 检查备份文件是否存在
        if os.path.isfile(backup_file_path):
            # 复制备份文件到原始文件位置
            shutil.copy2(backup_file_path, file_path)

    from .save import keep_specific_lines, ndi_del


    # 改变Order分别记录各测试集/各分割模型上的在predict/correct上的精密度/top15，记录标定矩阵
    # 仅用一张图像求解矩阵，优化方法使用powel。剔除异常图像后，在剩余图像上进行验证精度

    # 提前准备数据：①../imgs/row{}/imgs/标定图像数据集
    #             ②../data/ndi_read{}
    #             ③../data/p_end_phantom
    # if __name__ == '__main__':
    # def us_probe(file_order, cut_index, end_pose, model_wts):
    import time
    start_time = time.time()
    print("初始时间", start_time)
    # for o in range(5,15):
    #     if o==6:
    #         continue
    # =========分割图像，得到图像目标点集=============
    Order = file_order  # order表示哪个数据集
    # Order:  symbol="infer"分割模式（推理、训练、测试…）
    # cut_index = [155, 122, 433, 341]  # 绍兴迈瑞
    # cut_index = [171, 77, 438, 361]  # 天津海信
    cut_index = cut_index
    txt_path1 = dir + '/data/ndi_read{}.txt'.format(Order)
    check_and_replace_txt_file(txt_path1,dir+"/data/backup")
    segmented(model_wts, Order, batch_size=10, fold="output", symbol="infer", cut_index=cut_index,dir=dir)

    # =========对分割结果进行异常筛选=============
    # 用来存储选中图片的文件名（不带后缀）
    seg_folder_path = dir + "/imgs/out{}".format(Order)
    selected_images = []
    selected_images = read_images_from_folder(seg_folder_path, Order)
    selected_images.sort()
    # print(selected_images)

    # txt_path1 = dir + '/data/ndi_read{}.txt'.format(Order)
    # check_and_replace_txt_file(txt_path1,dir+"/data/backup")
    with open(txt_path1, 'r', encoding='utf-8') as file:  # 使用with语句自动管理文件打开和关闭
        line_count = sum(1 for line in file)  # 逐行读取并计数
    # if line_count == len(selected_images):
    #     txt_path1 = None

    txt_path2 = dir + '/data/us_points_C{}.txt'.format(Order)
    txt_path3 = dir + '/data/us_points_P{}.txt'.format(Order)
    txt_path4 = None
    print("101")
    out_files = get_filenames_without_extension(seg_folder_path)
    print("104")
    del_index = del_search(out_files, [i for i in range(len(os.listdir(dir + "/imgs/cut{}/imgs".format(Order))))])
    print("104")
    ndi_del(txt_path1, del_index)
    print("104")
    selected_images_0 = [out_files.index(i) for i in selected_images]
    print(selected_images_0)
    keep_specific_lines(selected_images_0, [txt_path1, txt_path2, txt_path3, txt_path4])
    print("102")
    return len(selected_images)

def us_probe(file_order,end_pose, batch_size,dir):
    return calculated(file_order, mode="correct", validation=0, batch_size=batch_size, p_ends_file=end_pose,dir=dir)


    # for mode in ["correct", "predit"]:
    #     calculated(Order, mode, validation=0, batch_size=len(selected_images), p_ends_file=end_pose)
    #     break
    # print("第{}标定时间".format(Order), time.time() - start_time)
    # start_time = time.time()


    # 验证不同优化算法下的预测/校正精密度（使用一张图像）
    # 验证不同数量下的预测/校正精密度（使用powel算法）
    # if __name__ == '__main__':
    #     Order = 0
    #     segmented(Order, batch_size=5, symbol="infer")
    #
    #     # 用来存储选中图片的文件名（不带后缀）
    #     seg_folder_path = "../imgs/out{}".format(Order)
    #     selected_images = []
    #     selected_images = read_images_from_folder(seg_folder_path,Order)
    #     print(selected_images)
    #
    #     txt_path1 = '../data/ndi_read{}.txt'.format(Order)
    #     with open(txt_path1, 'r', encoding='utf-8') as file:  # 使用with语句自动管理文件打开和关闭
    #         line_count = sum(1 for line in file)  # 逐行读取并计数
    #     if line_count == len(selected_images):
    #         txt_path1 = None
    #
    #     txt_path2 = '../data/us_points_C{}.txt'.format(Order)
    #     txt_path3 = '../data/us_points_P{}.txt'.format(Order)
    #     txt_path4 = None
    #     keep_specific_lines(selected_images, [txt_path1, txt_path2, txt_path3, txt_path4])
    #
    #     for mode in ["predit", "correct"]:
    #         calculated(Order,mode, validation=0, batch_size=len(selected_images))
    #
    #     for mode in ["predit", "correct"]:
    #         calculated(Order,mode, validation=1, batch_size=len(selected_images))

    # # 计算标定重复度
    # if __name__ == '__main__':
    #     matrix_list = np.zeros((6, 4, 4))
    #     for item in range(6):
    #         matrix_list[item] = np.loadtxt("../imgs/10/matrix{}.txt".format(item), delimiter=',')
    #     print("多个标定矩阵")
    #     print(matrix_list)
    #
    #     points_list = np.array([[0, 0, 0, 1], [0, 127, 0, 1], [64, 64, 0, 1], [127, 0, 0, 1], [127, 127, 0, 1]])
    #
    #     points_list_cal = np.zeros((6, 5, 4))
    #     for index, matrix in enumerate(matrix_list):
    #         points_list_cal[index] = (matrix @ points_list.T).T
    #     print("多个标定矩阵下的标记点坐标")
    #     print(points_list_cal)
    #
    #     averages = np.mean(points_list_cal, axis=0)
    #     print("多个标定矩阵下的标记点平均坐标")
    #     print(averages)
    #
    #     distance_loss = np.zeros((6, 5))
    #     # 遍历每一组数据（即matrix的每一行）
    #     for group_idx in range(6):
    #         # 对于当前组中的每个点（即matrix的每一列，但在这里我们使用点的索引进行迭代）
    #         for point_idx in range(5):
    #             # 计算当前点与平均值点集中对应点的距离
    #             # 注意：我们需要从averages_of_points中选择正确的点（即第point_idx个点）
    #             # 并且从matrix中选择当前组（第group_idx组）中的对应点
    #             point_in_group = points_list_cal[group_idx, point_idx, :]
    #             average_point = averages[point_idx, :]
    #             distance = np.sqrt(np.sum((point_in_group - average_point) ** 2))
    #             # 存储距离
    #             distance_loss[group_idx, point_idx] = distance
    #
    #     print("各组点与均值点之间的距离")
    #     print(distance_loss)
    #
    #     average_loss,std_loss = np.mean(distance_loss, axis=0),np.std(distance_loss, axis=0)
    #     print("各组点与均值点之间的平均距离")
    #     print(average_loss,std_loss)

    from .save import keep_specific_lines

    # # 计算测试集上的dice和distance
    # if __name__ == '__main__':
    #     # # 计算dice，得到pre-label有效配对点集===============================================
    #     # Order = "test"
    #     # batchsize = 10
    #     # for target in ["label", "output"]:
    #     #     segmented(Order, batch_size=batchsize, fold=target, symbol="test")
    #     #     if target == "label":
    #     #         pass
    #     #     if target == "output":
    #     #
    #     #         # 用来存储选中图片的文件名（不带后缀）
    #     #         seg_folder_path = "../imgs/out{}{}".format(target,Order)
    #     #         selected_images = []
    #     #         selected_images = read_images_from_folder(seg_folder_path,Order)
    #     #         print(selected_images)
    #     #
    #     #         txt_path1 = None
    #     #         txt_path2 = '../data/us_points_P{}{}{}.txt'.format("label",Order,batchsize)
    #     #         txt_path3 = '../data/us_points_P{}{}{}.txt'.format("output",Order,batchsize)
    #     #         txt_path4 = None
    #     #         keep_specific_lines(selected_images, [txt_path1, txt_path2, txt_path3, txt_path4])
    #
    #     def d_loss(array1,array2):
    #         n = array1.shape[0]
    #         # 初始化一个数组来存储每张图像的距离均值
    #         distance_means = np.zeros(n)
    #
    #         # 遍历每张图像
    #         for i in range(n):
    #             # 计算当前图像中所有对应点的距离
    #             # 使用numpy的广播机制来同时计算所有点的距离
    #             diff = array1[i, :, :] - array2[i, :, :]
    #             distances = np.sqrt(np.sum(diff ** 2, axis=1))  # axis=1表示沿着每个点的坐标轴求和
    #
    #             # 计算当前图像中所有点距离的平均值
    #             distance_means[i] = np.mean(distances)
    #
    #         # 此时，distance_means包含了每张图像内对应点的距离均值
    #         print(distance_means)
    #         print("max,min",np.max(distance_means),np.min(distance_means))
    #         print("mean,std",np.mean(distance_means),np.std(distance_means))
    #         print("mid,range",(np.max(distance_means)+np.min(distance_means))/2,np.max(distance_means)-np.median(distance_means))
    #
    #     # 计算distance================================================================
    #     for item in [2,5,10]:
    #         points_label = np.loadtxt("../data/us_points_Plabeltest{}.txt".format(item), delimiter=' ').reshape(-1,15,2)
    #         points_output = np.loadtxt("../data/us_points_Poutputtest{}.txt".format(item), delimiter=' ').reshape(-1,15,2)
    #         # print(points_label.shape)
    #         # print(points_label)
    #         # print(points_label.reshape(-1,15,2))
    #         d_loss(points_label,points_output)
    #         # break


from . import pre
def us_phantom(dir,filename):
    return pre.get_ends_pos(dir,filename)
