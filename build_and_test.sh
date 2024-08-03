#!/bin/bash

# 创建并进入 build 目录
mkdir -p build
cd build

# 运行 cmake 生成构建文件
cmake ..

# 编译项目
make

# 运行所有测试
ctest

# 返回根目录
cd ..
