cmake_minimum_required(VERSION 3.28)

# 项目名称和版本
project(remu VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加 src 目录下的源文件
add_executable(remu src/main.cpp)

# 设置构建类型为 Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# 添加 tests 目录中的 CMakeLists.txt
add_subdirectory(tests)

# 启用测试
enable_testing()
