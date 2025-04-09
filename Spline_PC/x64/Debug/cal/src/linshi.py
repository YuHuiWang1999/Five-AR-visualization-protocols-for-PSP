import torch
import torch.nn.functional as F

input_image = torch.randn(128, 128)  # 假设输入图像是一个形状为[batch_size, channels, height, width]的张量
output_image = F.interpolate(input_image, size=(256, 200), mode='bilinear', align_corners=False)
print(output_image.shape)