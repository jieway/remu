#!/bin/bash

# 定义容器名称和镜像
CONTAINER_NAME="cpp2c-dev"
IMAGE_NAME="clang-cpp2c"  # 必须与已构建的镜像名称一致

# 获取当前宿主机的绝对路径
HOST_DIR=$(pwd)

# 定义容器内的挂载路径
CONTAINER_DIR="/code"  # 更通用的目录名

# 运行容器并挂载目录
docker run -it --rm \
  --name $CONTAINER_NAME \
  -v "$HOST_DIR:$CONTAINER_DIR" \
  -w "$CONTAINER_DIR" \
  $IMAGE_NAME \
  "$@"  # 传递额外命令（如 clang++ -std=c++2c ...）