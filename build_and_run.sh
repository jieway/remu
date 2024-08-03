#!/bin/bash

# 创建并进入 build 目录
mkdir -p build
cd build

# 运行 cmake 生成构建文件
cmake ..

# 编译项目
make

# 运行项目可执行文件
if [ -f ./remu ]; then
  ./remu
else
  echo "Executable MyProject not found!"
fi

# 返回根目录
cd ..
