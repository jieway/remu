# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/tests)

# 添加单独的可执行文件，以 test_ 开头
add_executable(test_cpu test_cpu.cpp rv_test_helper.cpp)
add_executable(test_add test_add.cpp rv_test_helper.cpp)

# 添加测试
add_test(NAME test_cpu COMMAND test_cpu)
add_test(NAME test_add COMMAND test_add)
