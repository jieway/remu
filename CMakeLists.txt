# 🛠️ 声明构建该项目所需的最低 CMake 版本（保障兼容性）
cmake_minimum_required(VERSION 3.15)

# 📦 定义项目基本信息：名称 remu，版本 0.1.0
project(remu VERSION 0.1.0)

# 🎯 强制要求使用 C++23 标准（需编译器支持）
set(CMAKE_CXX_STANDARD 23)
# 🔒 明确声明必须满足上述 C++ 标准要求（否则构建失败）
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 📂 添加源代码目录（核心实现存放在 src 目录）
add_subdirectory(src)

# 🧪 添加测试代码目录（测试用例存放在 tests 目录）
add_subdirectory(tests)

# ✅ 启用项目的测试功能（需配合 ctest 命令使用）
enable_testing()

# -------------------------------------------------------------------
# ✨ 推荐扩展功能（可根据需要取消注释使用）：
# 
# 1. 编译器警告强化：
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
#
# 2. 调试符号生成：
# set(CMAKE_BUILD_TYPE Debug)
#
# 3. 添加编译特性检查：
# target_compile_features(remu PRIVATE cxx_std_23)
# -------------------------------------------------------------------