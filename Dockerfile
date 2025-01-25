# 使用 Ubuntu 官方镜像作为基础
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
    device-tree-compiler \
    && rm -rf /var/lib/apt/lists/*

# ======================
# 🔧 安装 LLVM/Clang 19
# ======================
RUN wget -qO- https://apt.llvm.org/llvm.sh | bash -s -- 19
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

# ======================
# 🚀 安装 RISC-V 工具链
# ======================
# 1. 安装 RISC-V GNU 工具链 (交叉编译)
RUN apt-get update && \
    apt-get install -y \
    gcc-riscv64-unknown-elf \
    gdb-multiarch \
    && rm -rf /var/lib/apt/lists/*

# 2. 安装 QEMU-RISCV
RUN apt-get update && \
    apt-get install -y \
    qemu-system-riscv64 \
    qemu-user \
    && rm -rf /var/lib/apt/lists/*

# ======================
# 📦 环境配置
# ======================
# 设置交叉编译工具链前缀
ENV RISCV_PREFIX=riscv64-unknown-elf-
ENV PATH="/opt/riscv/bin:${PATH}"

# 设置默认编译器环境变量
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++

# 创建工作目录
WORKDIR /workspace

# 清理缓存
RUN apt-get clean

# 设置默认命令
CMD ["/bin/bash"]