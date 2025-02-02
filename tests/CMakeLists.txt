# 🛠️ CMake 构建文件：用于配置测试程序的编译和链接流程

# 🧩 创建可执行测试程序 test_runner
# - test_main.cpp：测试主入口文件，负责启动并管理所有测试逻辑 ➡️ 🧪
# - ../src/cpu.cpp：被测 CPU 模块的源文件，是核心功能实现部分 ➡️ 🖥️
add_executable(test_runner  
    test_main.cpp      # 🧪 测试主入口（管理和执行测试逻辑）
    helpers.cpp        # 🧪 辅助函数实现文件
    ../src/cpu.cpp     # 🖥️ 被测 CPU 模块（核心功能实现，需要验证）
)  

# 🌐 添加头文件搜索路径
# PRIVATE 表示这些路径只对当前 target 生效，保证私有性和封装性
target_include_directories(test_runner  
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include  # 👉 指定 include 目录，存放公共头文件，供外部使用
        ${CMAKE_SOURCE_DIR}/src      # 👉 指定 src 目录，存放实现文件和部分私有头文件
)