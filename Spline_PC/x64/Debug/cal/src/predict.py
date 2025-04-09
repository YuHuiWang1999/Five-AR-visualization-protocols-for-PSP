import torch
from torch_geometric.loader import DataLoader
from .loss import Diceloss, Distanceloss
from tqdm import tqdm
from .shape import correct, get_centroids, correct_plot
import numpy as np
from .plot import point_in_img
from datetime import datetime
import cv2
from .save import save_pic, ensure_dir
import torch.nn.functional as F


# 验证模式下的模型预测=====================================================================================================
# 输出结果为：预测掩膜、校正点集、预测点集、点集间的距离差
def model_valid(model, model_weight, batch_size, image_size, graph_test_dataset, addition, epoc, order, fold,
                cut_index,dir):
    model = model().cuda()
    model.load_state_dict(torch.load(model_weight))
    # 构建数据加载器
    loader_graph = DataLoader(graph_test_dataset, batch_size=batch_size, shuffle=False)
    heigth = cut_index[2]-cut_index[0]
    width = cut_index[3]-cut_index[1]
    loss_total = 0
    index = 0
    output = None
    out_index = 0
    dis_loss = None
    res_ponits = []
    cur_batch = batch_size
    out_save = dir + "/imgs/output{}/".format(order)
    ensure_dir(out_save)
    output_save = dir + "/imgs/out{}/".format(order)
    ensure_dir(output_save)
    for batch_graph in loader_graph:
        # print("epoc",epoc)
        epoc = epoc - 1
        # 保留最后的batch
        if epoc < 0:
            cur_batch = addition
        # 每张三通道图视为一张图结构
        topk_batch = torch.zeros(cur_batch * image_size * image_size, dtype=torch.long).cuda()
        for item in range(cur_batch):
            topk_batch[item * image_size * image_size:(item + 1) * image_size * image_size] = item
            # 单张图结构的所有节点对应与batch中的一个索引。在处理批次图结构数据时，明确节点的归属

        # ①对输入图像进行预测分割==========================================================================================
        output = model(batch_graph.cuda(), topk_batch=topk_batch, data_batch=cur_batch, img_size=image_size)
        loss = Diceloss(output, batch_graph.y, cur_batch, image_size, mode="valid")
        # print("loss",loss)
        loss_total = loss_total + loss
        index = index + 1
        # print(index)

        output = F.interpolate(output, size=(width,heigth), mode='bilinear',
                                     align_corners=False)
        right = batch_graph.x.shape[0] // cur_batch
        for item in range(cur_batch):

            # correct_p包含校正点和预测点
            # 对手动提取的点进行校正
            if fold == "label":
                # 标签
                label_pic = batch_graph.y[right * item:right * (item + 1)].T.view(1, image_size,
                                                                                  image_size).detach().cpu().numpy().transpose(
                    1, 2,
                    0)
                correct_p, image_need = correct(label_pic)

            # ②对自动提取的点进行校正======================================================================================
            elif fold == "output":
                # 预测
                output_image = output[item].squeeze(0)
                # output_image = F.interpolate(output_image, size=(heigth, width), mode='bilinear',
                #                              align_corners=False)

                correct_p, image_need = correct(output_image,max(heigth,width))

                if isinstance(output_image, torch.Tensor):
                    image_out = output_image.detach().cpu().numpy() * 255
                else:
                    image_out = output_image * 255
                cv2.imwrite("{}{}.jpg".format(out_save, (index - 1) * batch_size + item), image_out)
            else:
                return

            # ③保存预测掩膜结果===========================================================================================
            # output_save = "../imgs/out{}/".format(order)
            # ensure_dir(output_save)

            # print((index - 1) * batch_size + item)

            # 只有有效轮廓数大于等于15的结果才会被保存
            if image_need is not None:
                # cv2.imshow("test", image_need)
                cv2.imwrite("{}{}.jpg".format(output_save, (index - 1) * batch_size + item), image_need)
                # cv2.imwrite("{}{}.jpg".format(output_save, out_index), image_need)
                # out_index += 1
                res_ponits.append(correct_p)
            else:
                continue

            # if correct_p is None:
            #     # 有效预测点少于15，视为异常，剔除该图像
            #     array_with_none = np.empty((2, 3, 5, 2), dtype=object)
            #     array_with_none.fill(0)
            #     res_ponits.append(array_with_none.tolist())
            #     point_in_img(None, np.zeros((128, 128, 3), dtype=np.uint8))
            #     continue
            # else:
            #     res_ponits.append(correct_p)
            # label_pic = batch_graph.y[right * item:right * (item + 1)].T.view(1, image_size,
            #                                                                   image_size).detach().cpu().numpy().transpose(
            #     1, 2,
            #     0)
            # 加上真值点
            # label_p = get_centroids(label_pic)
            # correct_p.append(label_p)
            # res_ponits.append(correct_p)
            # 绘制校正图
            # correct_plot(output[item].squeeze(0))
            # 绘制三点图
            # point_in_img(correct_p, label_pic)

        # 校正值，预测值

    res_ponits = np.array(res_ponits)
    print("=====运行到了这里22222====")
    print(res_ponits)
    # print("loss_total:{}".format(loss_total / index))
    # 校正值后与预测值
    y_cor, y_pre = res_ponits[:, 0, :, :], res_ponits[:, 1, :, :]

    dis_loss = Distanceloss(y_cor, y_pre)

    # y_pred, y_true,dis_loss,res_ponits = y_pred.tolist(), y_true.tolist(),dis_loss.tolist(),res_ponits.tolist()

    # print(y_cor)
    # ④采集点坐标保存到txt文件中===========================================================================================
    txt_file = dir + './data/us_points_C{}.txt'.format(order)
    np.savetxt(txt_file, y_cor.reshape(-1, 30), fmt='%d')
    txt_file = dir + './data/us_points_P{}.txt'.format(order)
    np.savetxt(txt_file, y_pre.reshape(-1, 30), fmt='%d')
    return output, res_ponits, dis_loss


# 几乎通验证模式下的模型预测================================================================================================
def model_test(model, model_weight, batch_size, image_size, graph_test_dataset, order, fold):
    model = model().cuda()
    model.load_state_dict(torch.load(model_weight))
    # 构建数据加载器
    loader_graph = DataLoader(graph_test_dataset, batch_size=batch_size, shuffle=False)
    loss_total = 0
    res_ponits = []
    for index, batch_graph in enumerate(loader_graph):
        # 每张三通道图视为一张图结构
        topk_batch = torch.zeros(batch_size * image_size * image_size, dtype=torch.long).cuda()
        for item in range(batch_size):
            topk_batch[item * image_size * image_size:(item + 1) * image_size * image_size] = item
            # 单张图结构的所有节点对应与batch中的一个索引。在处理批次图结构数据时，明确节点的归属

        output = model(batch_graph.cuda(), topk_batch=topk_batch, data_batch=batch_size, img_size=image_size)
        loss = Diceloss(output, batch_graph.y, batch_size, image_size, mode="valid")
        # print("loss",loss)
        loss_total = loss_total + loss

        right = batch_graph.x.shape[0] // batch_size
        for item in range(batch_size):
            # correct_p包含校正点和预测点
            if fold == "label":
                # 标签
                label_pic = batch_graph.y[right * item:right * (item + 1)].T.view(1, image_size,
                                                                                  image_size).detach().cpu().numpy().transpose(
                    1, 2,
                    0)
                correct_p, image_need = correct(label_pic)
            if fold == "output":
                # 预测
                correct_p, image_need = correct(output[item].squeeze(0))

            # output_save = "../data/test/output_C{}/".format(target)
            output_save = "./imgs/out{}{}{}/".format(fold, order, batch_size)

            ensure_dir(output_save)
            if image_need is not None:
                # cv2.imshow("test", image_need)
                cv2.imwrite("{}{}.jpg".format(output_save, index * batch_size + item), image_need)

            if correct_p == None:
                # 有效预测点少于15，视为异常，剔除该图像
                array_with_none = np.empty((2, 3, 5, 2), dtype=object)
                array_with_none.fill(0)
                res_ponits.append(array_with_none.tolist())
                point_in_img(None, np.zeros((128, 128, 3), dtype=np.uint8))
                continue
            else:
                res_ponits.append(correct_p)

        # 校正值，预测值，真实值
    res_ponits = np.array(res_ponits)
    # 校正值后与预测值
    y_cor, y_pre = res_ponits[:, 0, :, :], res_ponits[:, 1, :, :]
    # print("loss_total:{}".format(loss_total / (index + 1)))
    # 采集点坐标保存到txt文件中
    # txt_file = '../data/test/{}_C{}.txt'.format(fold, target)
    # np.savetxt(txt_file, y_cor.reshape(batch_size * (index + 1), 30), fmt='%d')
    # txt_file = '../data/test/{}_P{}.txt'.format(fold, target)
    # np.savetxt(txt_file, y_pre.reshape(batch_size * (index + 1), 30), fmt='%d')

    txt_file = '../data/us_points_C{}{}{}.txt'.format(fold, order, batch_size)
    np.savetxt(txt_file, y_cor.reshape(-1, 30), fmt='%d')
    txt_file = '../data/us_points_P{}{}{}.txt'.format(fold, order, batch_size)
    np.savetxt(txt_file, y_pre.reshape(-1, 30), fmt='%d')
    return output, res_ponits


# 验证模式下的模型预测=====================================================================================================
def model_train(model, batch_size, graph_train_dataset, graph_test_dataset, image_size, Epoch=500):
    model = model().cuda()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.01)
    # 构建数据加载器
    loader_graph = DataLoader(graph_train_dataset, batch_size=batch_size, shuffle=True)
    loader_graph_test = DataLoader(graph_test_dataset, batch_size=batch_size, shuffle=True)
    early_stopping = EarlyStopping()
    min_loss = float('inf')
    now = datetime.now()
    current_time = now.strftime("%m%d")
    # 使用模型进行前向传播
    for epoch in tqdm(range(Epoch)):
        print("======={}=======================".format(epoch))
        train_total = 0
        valid_total = 0
        index = 0
        model.train()
        for batch_graph in loader_graph:
            # 每张三通道图视为一张图结构
            topk_batch = torch.zeros(batch_size * image_size * image_size, dtype=torch.long).cuda()
            for item in range(batch_size):
                topk_batch[item * image_size * image_size:(item + 1) * image_size * image_size] = item

            model.zero_grad()
            output = model(batch_graph.cuda(), topk_batch=topk_batch, data_batch=batch_size, img_size=image_size)

            loss_t = Diceloss(output, batch_graph.y, batch_size, image_size, mode="train")
            train_total = train_total + loss_t

            loss_t.backward()
            optimizer.step()
            optimizer.zero_grad()

            loss_v = Diceloss(output, batch_graph.y, batch_size, image_size, mode="valid")
            valid_total = valid_total + loss_v

            index = index + 1
            # break
        # print("loss_train:{}========================================================================"
        # .format(valid_total / index))
        # break

        valid_total = 0
        index = 0
        model.eval()
        for batch_graph in loader_graph_test:
            # 每张三通道图视为一张图结构
            topk_batch = torch.zeros(batch_size * image_size * image_size, dtype=torch.long).cuda()
            for item in range(batch_size):
                topk_batch[item * image_size * image_size:(item + 1) * image_size * image_size] = item

            output = model(batch_graph.cuda(), topk_batch=topk_batch, data_batch=batch_size, img_size=image_size)
            loss_v = Diceloss(output, batch_graph.y, batch_size, image_size, mode="valid")
            valid_total = valid_total + loss_v
            index = index + 1
            # break
        # print("loss_test:{}========================================================================"
        #       .format(valid_total / index))
        # break

        # 早停策略
        early_stopping(valid_total / index)
        if early_stopping.early_stop:
            print("Early stopping")
            break

        if min_loss > (valid_total / index):
            min_loss = valid_total / index
            print("min_loss:{}".format(min_loss))
            torch.save(model.state_dict(), '../wts/my_model_{}_{}.pth'.format(batch_size, current_time))


# 训练模式下的早停策略=====================================================================================================
class EarlyStopping:
    """Early stops the training if validation loss doesn't improve after a given patience."""

    # def __init__(self, patience=7, delta=0):
    def __init__(self, patience=50, delta=0):
        """
        Args:
            patience (int): How long to wait after last time validation loss improved.
                            上次验证集损失值改善后等待几个epoch
                            Default: 7
            delta (float): Minimum change in the monitored quantity to qualify as an improvement.
                            监测数量的最小变化，以符合改进的要求
                            Default: 0
        """
        self.patience = patience
        self.delta = delta

        self.counter = 0
        self.best_score = None
        self.early_stop = False

    def __call__(self, val_loss):

        score = -val_loss

        if self.best_score is None:
            self.best_score = score
            # self.save_checkpoint(val_loss, model)
        elif score < self.best_score + self.delta:
            self.counter += 1
            # print(f'EarlyStopping counter: {self.counter} out of {self.patience}')
            if self.counter >= self.patience:
                self.early_stop = True
                print("Early", self.best_score)
        else:
            self.best_score = score
            # self.save_checkpoint(val_loss, model)
            self.counter = 0
