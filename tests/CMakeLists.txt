# 🧩 创建可执行测试程序  
add_executable(test_runner  
    test_main.cpp      # 🧪 测试主入口（测试逻辑载体）  
    ../src/cpu.cpp     # 🖥️ 被测CPU模块源码（需验证的核心功能）  
)  

# 🌐 添加头文件搜索路径  
target_include_directories(test_runner  
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include  # 👉 使用 include 目录
        ${CMAKE_SOURCE_DIR}/src      # 👉 私有头文件可以保留在 src
)