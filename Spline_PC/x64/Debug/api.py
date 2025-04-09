from segment import segment_api_single
from segment import segment_api_group
from segment import instantiate
import os
def preparation(dir1):
    instantiate(dir1)
    return 1

def seg_single(dir2,index):
# def main():
#     dir1 = r"A:\Desktop_me\Cpython0511single\Cpython0501\final.pt"
#     dir2 = r"B:\deep_mrs\uuss\img\a10.png"
#     dir3 = r"A:\Desktop_me\Cpython0511single\Cpython0501\x64\output"
#     index = [0,0,448,448]
    if os.path.isdir(dir2):
        print("错误：选择的对象可能为一个文件夹")
        return 1
    segment_api_single(dir2,index)
    return 1

def seg_group(dir2,index):
# def main():
#     dir1 = r"A:\Desktop_me\Cpython0511single\Cpython0501\final.pt"
#     dir2 = r"B:\deep_mrs\uuss\img\a10.png"
#     dir3 = r"A:\Desktop_me\Cpython0511single\Cpython0501\x64\output"
#     index = [0,0,448,448]
    if os.path.isfile(dir2):
        print("错误：选择的对象可能为一个文件")
        return 1
    segment_api_group(dir2,index)
    return 1

# main()