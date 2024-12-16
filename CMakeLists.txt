cmake_minimum_required(VERSION 3.15)
project(remu VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加子目录
add_subdirectory(src)
add_subdirectory(tests)

# 启用测试
enable_testing()