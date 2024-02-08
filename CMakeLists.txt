cmake_minimum_required(VERSION 3.27)
project(crvemu)

set(CMAKE_CXX_STANDARD 23)

# 公共部分
set(COMMON_SOURCES
        param.h
        dram.h
        dram.cpp
        bus.h
        bus.cpp
        cup.h
        cup.cpp
        exception.h
        exception.cpp
)

# 库
add_library(common_library ${COMMON_SOURCES})

# 可执行文件
add_executable(crvemu
        main.cpp
)

# 将库链接到 cemu 可执行文件
target_link_libraries(crvemu common_library)

# 启用测试并添加测试文件
enable_testing()
add_subdirectory("third_party/googletest")

add_executable(g_test
        g_test.cc
)

# 将库链接到 g_test 可执行文件
target_link_libraries(g_test common_library GTest::gtest_main)

# 配置 GoogleTest
include(GoogleTest)
gtest_discover_tests(g_test)
