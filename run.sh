#!/bin/bash

# ===================================================
# 🛠️ RISC-V & C++26 Development Environment Launcher
# ===================================================

# Container configuration
CONTAINER_NAME="remu-dev"   # Container name
IMAGE_NAME="remu"           # Docker image name
CONTAINER_DIR="/code"       # Container mount directory

# ===================================================
# 🔍 Check Docker image existence
# ===================================================
if [[ -z "$(docker images -q $IMAGE_NAME 2>/dev/null)" ]]; then
    echo "🚀 Building image $IMAGE_NAME..."
    if ! docker build -t $IMAGE_NAME .; then
        echo "❌ Build failed! Please check Dockerfile"
        exit 1
    fi
    echo "✅ Image built successfully!"
else
    echo "🔍 Using existing image $IMAGE_NAME"
fi

# ===================================================
# 🚀 Launch development container
# ===================================================
echo "Starting container $CONTAINER_NAME..."
# 修改后的启动命令（移除 -u 参数）
docker run -it --rm \
  --name "$CONTAINER_NAME" \
  -v "$(pwd):$CONTAINER_DIR" \
  -w "$CONTAINER_DIR" \
  "$IMAGE_NAME" \
  "$@"

# ===================================================
# 📝 Usage Examples
# ===================================================
# 1. Interactive shell:
#   ./run.sh
#
# 2. Compile & run C++ code:
#   ./run.sh clang++ -std=c++2c -o main main.cpp && ./run.sh ./main
#
# 3. Cross-compile RISC-V:
#   ./run.sh riscv64-unknown-elf-gcc -O2 -o firmware.elf main.c