import cv2
import numpy as np
import matplotlib.pyplot as plt
def cut_img(dir,symbol=1,alpha=2,beta=50):
    cap0 = cv2.VideoCapture(symbol)  # 读取摄像头视频
    while cap0.isOpened():
        ret, frame = cap0.read()
        width = frame.shape[0]  # 获取视频的宽度
        height = frame.shape[0]  # 获取视频的高度
        frame_shot = np.clip(frame, 0, 255).astype(np.uint8)
        cv2.imshow("Video frame saving", frame_shot)
        cv2.waitKey(0)  # 等待用户按键，按任意键关闭窗口
        cv2.destroyAllWindows()  # 销毁所有OpenCV窗口
        cv2.imwrite(dir+"/wts/test.jpg", frame_shot)
        print("保存测试图片：成功")
        break

    # 回调函数，用于捕获鼠标点击事件
    def mouse_callback(event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:  # 当鼠标左键点击时
            print(f"Clicked at ({x}, {y})")
            # mouse_clicks.append((x, y))
            mouse_clicks.append(x)
            mouse_clicks.append(y)

    # 创建一个空数组用于保存鼠标点击坐标
    mouse_clicks = []
    # 读取图片
    image_path = dir+'/wts/test.jpg'
    test_image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

    # 增强对比度
    # 这里使用简单的线性变换作为示例，你可以根据需要调整alpha值来增强对比度
    alpha = alpha # 对比度控制（1.0-3.0）
    beta = beta
    test_image = cv2.convertScaleAbs(test_image, alpha=alpha, beta=beta)

    # 在图片上设置鼠标事件回调
    cv2.namedWindow('Imaging range selection')
    cv2.setMouseCallback('Imaging range selection', mouse_callback)

    while True:
        cv2.imshow('Imaging range selection', test_image)
        key = cv2.waitKey(1) & 0xFF
        # 按 'q' 键退出循环
        if key == 27:
            break

    # 打印并显示保存的鼠标点击坐标
    print("Mouse clicks:", mouse_clicks)
    # 释放窗口
    cv2.destroyAllWindows()

    image_cut = test_image[mouse_clicks[1]:mouse_clicks[3],mouse_clicks[0]:mouse_clicks[2]]

    # cut_index = '../wts/cut_index.txt'
    # with open(cut_index, 'w') as file:
    #     for i, item in enumerate(mouse_clicks):
    #         file.write("{}. {}\n".format(i + 1, item))
    np.savetxt(dir+r'/wts/cut_index.txt', mouse_clicks, fmt='%f', delimiter=',')

    print(image_cut.shape)
    cv2.imshow("image_cut", image_cut)
    cv2.waitKey(0)  # 等待用户按键，按任意键关闭窗口
    cv2.destroyAllWindows()  # 销毁所有OpenCV窗口

if __name__ == '__main__':
    cut_img()



