import math
import torch
from .shape import counted
import numpy as np


def Diceloss(y_pred, y_true, batch_size, image_size, mode=None):
    Loss = 0
    for i in range(batch_size):
        # 把数组拉直成向量
        # p = y_pred[b*img_size*img_size:(b+1)*img_size*img_size]
        pred = torch.flatten(y_pred[i].squeeze(0))
        mask = torch.flatten(y_true[i * image_size * image_size:(i + 1) * image_size * image_size])
        # print("shape", pred.shape, mask.shape)
        # 计算交集,mask中像素值为0或1
        overlap = (pred * mask).sum()
        denum = pred.sum() + mask.sum() + 1e-8
        dice = (2 * overlap) / denum
        loss = 1 - dice
        # print("loss:",loss)
        # print("y_pred[b].shape:", y_pred[b].squeeze(0).shape)
        if mode == "valid":
            Loss += loss
            # print("Loss:",Loss)
        else:
            num, _, _, _ = counted(y_pred[i].squeeze(0))
            num = torch.tensor(num).float()
            # print("num, s:", num, s)
            loss = math.exp(abs(num - 15)) * loss
            Loss += loss
            # print("Loss:",Loss)
            # if num != 15:
            #     loss = math.exp(abs(min(15,(num - 15)))) * 2 * loss
            #     # print("loss_n15:", loss)
            # else:
            #     loss = 1 * (tanh(s) + 1) * loss
            #     # print("loss_15:", loss)

    return Loss / batch_size


def Distanceloss(y_pred, y_true):
    if y_pred.shape != y_true.shape:
        raise ValueError("predicted和actual的形状必须相同")

    # 初始化误差数组
    errors = np.zeros_like(y_pred[..., 0])

    # 遍历每个点计算误差D

    for i in range(y_pred.shape[0]):
        for j in range(y_pred.shape[1]):
            for k in range(y_pred.shape[2]):
                if y_pred[i, j, k][0] is None or y_true[i, j, k][0] is None:
                    errors[i, j, k] = None
                    continue

                diff = y_pred[i, j, k] - y_true[i, j, k]
                # 向量的欧几里得长度
                errors[i, j, k] = np.linalg.norm(diff)

    return errors
