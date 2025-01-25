#!/bin/bash

# 如果build目录存在，则删除它
if [ -d "build" ]; then
    echo "Cleaning build directory..."
    rm -rf build
fi

# 创建新的构建目录
mkdir -p build
cd build

# 输出分隔线函数
print_separator() {
    echo "----------------------------------------"
}

# 执行 CMake 构建
echo "Configuring project with CMake..."
cmake ..

print_separator

# 编译项目
echo "Building project..."
cmake --build .

# 检查编译是否成功
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

print_separator

# 运行测试程序
echo "Running tests:"
./tests/test_runner

print_separator

# 运行主程序
echo "Running main program:"
./src/remu

print_separator

echo "All done!"