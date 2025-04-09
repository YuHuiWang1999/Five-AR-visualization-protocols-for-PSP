import cv2
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import torch

# ①读取分割掩膜，对其中的点进行计数==========================================================================================
def counted(image_filename):
    # 1. 读取图像
    if type(image_filename) == str:
        # print("string")
        image = cv2.imread(image_filename, cv2.IMREAD_COLOR)
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        # print("image.shape, gray.shape", image.shape, gray.shape)

    else:
        # print("tensor")
        if isinstance(image_filename, torch.Tensor):
            image = image_filename.detach().cpu().numpy() * 255
        else:
            image = image_filename * 255
        # plt.figure()
        # plt.imshow(image, cmap='gray')
        # plt.axis('off')  # 关闭坐标轴
        # plt.show()
        # image = (image_filename.detach().cpu().numpy() * 255).astype(np.uint8)
        image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR).astype(np.uint8)
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        # gray = image*255
        # print("max(image),min(image)", np.max(image), np.min(image))
        # print("image.shape, gray.shape", image.shape, gray.shape)

    # 2. 转换为灰度图像,添加高斯模糊
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    # print("max(gray),min(gray)", np.max(gray), np.min(gray))
    # 3. 应用阈值操作或边缘检测来创建二值图像
    _, thresh = cv2.threshold(blurred, 100, 255, cv2.THRESH_BINARY)
    # print("max(thresh),min(thresh)", np.max(thresh), np.min(thresh))
    # 4. 查找轮廓
    contours, _ = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 5. 计算每个轮廓的面积
    contour_areas = [cv2.contourArea(contour) for contour in contours]

    # 6. 计算轮廓面积，筛选出面积最大的n个轮廓
    sorted_contours = [x for _, x in sorted(zip(contour_areas, contours), key=lambda pair: pair[0], reverse=True)]
    top_contours = sorted_contours[:15]

    # # 绘制最大的前十五个轮廓
    # # print(image.shape)
    # cv2.drawContours(image, top_contours, -1, (0, 255, 0), 3)
    # # 显示结果
    # cv2.imshow('Image with Top 15 Contours', image)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    return len(sorted_contours), -1, top_contours, image


# ②获取标定点的质心坐标====================================================================================================
def get_centroids(input_data):
    ponits_num, _, top15_points, image = counted(input_data)

    # if len(top15_points) == 0:
    #     return 0, -1, None
    if len(top15_points) < 15:
        return len(top15_points), -1, None

    # 7. 计算每个轮廓的质心
    centroids = []
    for contour in top15_points:
        # 计算轮廓的矩
        M = cv2.moments(contour)
        # 检查m00（面积）是否不为零
        if M["m00"] != 0.0:
            # 计算质心
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])

            # 将质心添加到列表中
            # 坐标系不同，因此此处的X与Y互换
            centroids.append([cY, cX])
        else:
            print("面积为零")
            return len(top15_points), -1, None
    # print(len(centroids))
    # print(centroids)
    # 8. 15个质心划归到三行五列
    rows, cols = 3, 5
    grid_centroids = []
    centroids = sorted(centroids, key=lambda point: (point[0]))
    # print(len(centroids))
    for row in range(rows):
        row_ = sorted(centroids[cols * row: cols * (row + 1)], key=lambda point: (point[1]))
        # print("row_", row_)
        grid_centroids.append(row_)

    # print("centroids", centroids)
    # print("grid_centroids", grid_centroids)

    # # 8.1 15个质心划归到五行三列
    # rows, cols = 5, 3
    # grid_centroids = []
    # centroids = sorted(centroids, key=lambda point: (point[1]))
    # for row in range(rows):
    #     row_ = sorted(centroids[cols * row: cols * (row + 1)], key=lambda point: (point[0]))
    #     grid_centroids.append(row_)

    # 9. 可视化测试，在原始图像上绘制这些轮廓
    for contour in top15_points:
        cv2.drawContours(image, [contour], -1, (0, 255, 255), 1)
    for centroid in centroids:
        # print(image[centroid[0], centroid[1]])
        # print(image[centroid[0], centroid[1]].shape)
        image[centroid[0], centroid[1]] = [0, 0, 255]

    # # 可视化
    # plt.figure()
    # plt.imshow(image, cmap='gray')
    # plt.title('image')
    # plt.axis('off')  # 关闭坐标轴
    # plt.show()

    image_need = image

    # print(len(grid_centroids), len(grid_centroids[0]), len(grid_centroids[1]), len(grid_centroids[2]))
    return grid_centroids, 1, image_need


# ③对质心坐标进行校正=====================================================================================================
def correct(input_data,range_l):
    # ponits_num, _, top15_points, image = counted(input_data)
    # grid_centroids = get_centroids(top15_points, image)

    grid_centroids, sym, image_need = get_centroids(input_data)
    if sym == -1:
        return None,None

    # 记录斜率的集合
    m_list = []
    b_list = []
    # 循环遍历每组数据
    for i, data in enumerate(grid_centroids):
        points = np.array(data)
        # print("data", points)
        # 坐标系不同，因此此处的X与Y互换
        y = points[:, 0]
        x = points[:, 1]

        # 计算直线的斜率和截距
        n = len(x)
        sum_x = np.sum(x)
        sum_y = np.sum(y)
        sum_xy = np.sum(x * y)
        sum_x2 = np.sum(x ** 2)

        m = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x ** 2)
        b = (sum_y - m * sum_x) / n
        m_list.append(m)
        b_list.append(b)

    m_average = sum(m_list) / 3  # 斜率取平均值，待考虑

    grid_corrects = []
    for i, data in enumerate(grid_centroids):
        points = np.array(data)
        # print("data", points)
        # 坐标系不同，因此此处的X与Y互换
        y = points[:, 0]
        x = points[:, 1]

        # 计算投影点的坐标
        projected_points = np.zeros_like(points)
        projected_points[:, 0] = x  # x坐标保持不变
        projected_points[:, 1] = m_list[i] * x + b_list[i]  # 使用自身斜率
        # projected_points[:, 1] = m_average * x + b_list[i]  # 使用平均斜率替代
        projected_points = np.clip(projected_points, 0, range_l)

        grid_corrects.append(projected_points)
    # print(len(grid_corrects), len(grid_corrects[0]), len(grid_corrects[1]), len(grid_centroids[2]))
    grid_corrects = np.array(grid_corrects)
    grid_corrects[:, :, [0, 1]] = grid_corrects[:, :, [1, 0]]
    # 校正后的点集、未校正点集，图像
    return [grid_corrects.tolist(), grid_centroids], image_need

# 对质心校正效果的可视化====================================================================================================
def correct_plot(input_data):
    grid_centroids = get_centroids(input_data)

    # 10.拟合直线
    # 设置全局字体大小
    mpl.rcParams['font.size'] = 20
    # 初始化颜色列表，用于区分不同数据集
    colors = ['b', 'g', 'r', 'c', 'm']
    markers = ['o', 's', '^', 'D', 'p']  # 不同的标记样式
    # 初始化图形
    plt.figure(figsize=(10, 10))
    # 设置纵轴的坐标范围 ymin 到 ymax
    ymin, ymax = 0, 128
    plt.ylim(ymin, ymax)
    plt.gca().invert_yaxis()  # 反转y轴方向
    # 设置横轴的坐标范围 xmin 到 xmax
    xmin, xmax = 0, 128
    plt.xlim(xmin, xmax)
    # 添加图例和坐标轴标签
    plt.xlabel('X-axis')
    plt.ylabel('Y-axis')
    plt.title('Linear Fit and Projection of Five Data Sets')

    # 记录斜率的集合
    m_list = []
    b_list = []
    # 循环遍历每组数据
    for i, data in enumerate(grid_centroids):
        points = np.array(data)
        # print("data", points)
        # 坐标系不同，因此此处的X与Y互换
        y = points[:, 0]
        x = points[:, 1]

        # 计算直线的斜率和截距
        n = len(x)
        sum_x = np.sum(x)
        sum_y = np.sum(y)
        sum_xy = np.sum(x * y)
        sum_x2 = np.sum(x ** 2)

        m = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x ** 2)
        b = (sum_y - m * sum_x) / n
        m_list.append(m)
        b_list.append(b)

    m_average = sum(m_list) / 3

    grid_corrects = []
    for i, data in enumerate(grid_centroids):
        points = np.array(data)
        # print("data", points)
        # 坐标系不同，因此此处的X与Y互换
        y = points[:, 0]
        x = points[:, 1]

        # 计算投影点的坐标
        projected_points = np.zeros_like(points)
        projected_points[:, 0] = x  # x坐标保持不变
        projected_points[:, 1] = m_average * x + b_list[i]  # 计算y坐标

        # 绘制原始点===============================
        plt.scatter(x, y, color=colors[i], label=f'Original Points {i + 1}', marker=markers[i], facecolors=colors[i])

        # 绘制拟合直线=============================
        x_fit = np.linspace(min(x), max(x), 100)
        y_fit = m_average * x_fit + b_list[i]
        plt.plot(x_fit, y_fit, color=colors[i], alpha=0.5, label=f'Fitted Line {i + 1}')

        # 绘制原始点到投影点的垂直线段（虚线）=========
        for x_, y_, y_proj_ in zip(x, y, projected_points[:, 1]):
            plt.plot([x_, x_], [y_, y_proj_], 'r--', linewidth=1)  # 红色虚线，线宽为1
        # 绘制投影点===============================
        plt.scatter(projected_points[:, 0], projected_points[:, 1], color=colors[i], label=f'Projected Points {i}',
                    marker=markers[i], facecolors='none', edgecolors=colors[i])

        # print((x, y), (projected_points[:, 0], projected_points[:, 1]))

        # 计算每个点到直线的垂直距离
        distances = np.abs(m_average * x - y + b_list[i]) / np.sqrt(m_average ** 2 + 1)
        # 设置离群点的阈值
        threshold = np.std(distances) * 2  # 根据你的数据设定合适的阈值
        # 找到离群点的索引
        outliers_indices = np.where(distances > threshold)[0]
        # 如果存在离群点，用五角星标记它们
        if outliers_indices.size > 0:
            outliers_x = x[outliers_indices]
            outliers_y = y[outliers_indices]
            plt.scatter(outliers_x, outliers_y, color="black", label=f'Outliers {i + 1}', marker='*', linewidths=2)

        grid_corrects.append(projected_points)

    # 显示图形
    # plt.legend()
    plt.show()

    return grid_corrects
