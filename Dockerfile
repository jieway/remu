# 使用 Ubuntu 23.04 官方镜像作为基础
FROM ubuntu

# 设置非交互式环境变量（避免安装过程中的提示）
ENV DEBIAN_FRONTEND=noninteractive

# 更新系统并安装必要工具
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    wget \
    gnupg \
    software-properties-common \
    cmake \
    ninja-build \
    git \
    && rm -rf /var/lib/apt/lists/*

# 添加 LLVM 官方仓库（支持最新 Clang 版本）
RUN wget -qO- https://apt.llvm.org/llvm.sh | bash -s -- 19

# 安装 Clang 19 及相关工具
RUN apt-get update && \
    apt-get install -y \
    clang-19 \
    lldb-19 \
    lld-19 \
    clangd-19 \
    libc++-19-dev \
    libc++abi-19-dev \
    && rm -rf /var/lib/apt/lists/*

# 设置 Clang 19 为默认编译器
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-19 100 && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-19 100 && \
    update-alternatives --install /usr/bin/lld lld /usr/bin/lld-19 100

# 设置环境变量（可选）
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++

# 创建工作目录
WORKDIR /app

# 示例：编译代码（按需修改）
# COPY . /app
# RUN clang++ -std=c++2c -o main main.cpp

# 清理缓存（可选）
RUN apt-get clean

# 设置默认命令（可按需修改）
CMD ["/bin/bash"]