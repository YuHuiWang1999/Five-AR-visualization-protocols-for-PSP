'''生成新文件名'''
import os


def check_filename(filename):
    # 检查文件是否存在
    if os.path.exists(filename):
        # 拆分文件名和扩展名
        name, ext = os.path.splitext(filename)
        # 创建一个新的文件名-在文件名末尾添加数字(1, 2, 3...)
        i = 1
        while os.path.exists(f"{name[:-1]}{i}{ext}"):
            i += 1
        new_filename = f"{name[:-1]}{i}{ext}"
    else:
        new_filename = filename

    # print(new_filename)
    return new_filename


if __name__ == '__main__':
    # 调用保存文件函数
    new_filename = check_filename("./usshujv/1")
