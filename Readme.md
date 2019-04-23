# 用途
用于测试file的一些特性
test_file.cpp: 测试获取文件长度
generate_file.cpp: 测试创建文件夹和若干文件，然后扫描文件夹、获得最大序列号，并计算时间。

# 编译
g++ test_file.cpp -o test
g++ generate_file.cpp -o g -std=c++11
