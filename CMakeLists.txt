cmake_minimum_required(VERSION 3.27)
project(crvemu)

set(CMAKE_CXX_STANDARD 23)

# 公共部分
set(COMMON_SOURCES
        src/param.h
        src/dram.h
        src/dram.cpp
        src/bus.h
        src/bus.cpp
        src/cup.h
        src/cup.cpp
        src/instructions.h
        src/instructions.cpp
)

# 库
add_library(common_library ${COMMON_SOURCES})

# 可执行文件
add_executable(crvemu
        src/main.cpp
)

# 将库链接到 crvemu 可执行文件
target_link_libraries(crvemu common_library)

# 启用测试并添加测试文件
enable_testing()
add_subdirectory("third_party/googletest")

add_executable(unit_test
        tests/instructions_test.cpp
        tests/test_util.h
        tests/test_util.cpp
)

# 将库链接到 unit_test 可执行文件
target_link_libraries(unit_test common_library GTest::gtest_main)

# 配置 GoogleTest
include(GoogleTest)
gtest_discover_tests(unit_test)

