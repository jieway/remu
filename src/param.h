#include <cstddef> // 引入定义 std::size_t 的头文件

namespace cemu {
    // 使用 std::size_t 替代 uint64_t 来表示内存大小，更好地表达意图和增加可移植性
    constexpr std::size_t DRAM_BASE = 0x8000'0000;

    // 定义DRAM的大小，128MB
    constexpr std::size_t DRAM_SIZE = 1024 * 1024 * 128;

    // 定义DRAM的结束地址
    constexpr std::size_t DRAM_END = DRAM_SIZE + DRAM_BASE - 1;
}