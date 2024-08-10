# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/tests)

# 添加 test_main.cpp 文件到可执行文件 runTests 中
add_executable(test_cpu test_cpu.cpp)

# 添加测试
add_test(NAME runTests COMMAND runTests)
