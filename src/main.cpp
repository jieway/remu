#include <array>
#include <cstdint>
#include <vector>

// 定义DRAM_SIZE为128MB
const uint64_t DRAM_SIZE = 1024 * 1024 * 128;

class Cpu {
  // RISC-V 有 32 个寄存器
  std::array<uint64_t, 32> regs;

  // PC 寄存器包含下一条指令的内存地址
  uint64_t pc;

  // 内存，一个字节数组。在真实的 CPU 中没有内存，这里仅作模拟。
  std::vector<uint8_t> dram;

 public:
  // 构造函数
  Cpu(const std::vector<uint8_t>& code) : pc(0), dram(code) {
    regs.fill(0);             // 初始化寄存器为0
    regs[2] = DRAM_SIZE - 1;  // 设置堆栈指针寄存器的初始值
  }

  // 可能需要的其他成员函数声明
};

int main() {
  // 示例代码使用
  std::vector<uint8_t> code = {/* 初始化代码 */};
  Cpu cpu(code);
  // 使用cpu对象进行操作
}