cmake_minimum_required(VERSION 3.27)
project(cemu)

# 设置C++标准为C++20
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts")

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
        src/log.h
        src/csr.h
        src/csr.cpp
        src/exception.cpp
        src/exception.h
        src/plic.cpp
        src/plic.h
        src/clint.cpp
        src/clint.h
)

add_library(common_library ${COMMON_SOURCES})

# 可执行文件
add_executable(cemu
        src/main.cpp
)

# 将库链接到 cemu 可执行文件
target_link_libraries(cemu common_library)

# 启用测试并添加测试文件
enable_testing()
add_subdirectory("third_party/googletest")

add_executable(unit_test
        tests/unitest/instructions_test.cpp
        tests/test_util.h
        tests/test_util.cpp
        tests/unitest/dram_test.cpp
        tests/unitest/bus_test.cpp
        tests/unitest/cpu_test.cpp
        tests/unitest/csr_test.cpp
        tests/unitest/exception.cpp
        tests/unitest/plic_test.cpp
        tests/unitest/client_test.cpp
)

# 将库链接到 unit_test 可执行文件
target_link_libraries(unit_test common_library GTest::gtest_main)

# 配置 GoogleTest
include(GoogleTest)
gtest_discover_tests(unit_test)