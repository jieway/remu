#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

// DRAM_SIZE定义了内存的大小为128MB
const uint64_t DRAM_SIZE = 1024 * 1024 * 128;

// Cpu类定义
class Cpu {
  // regs数组表示32个寄存器
  std::array<uint64_t, 32> regs;
  // pc表示程序计数器
  uint64_t pc;
  // dram表示动态随机存取存储器
  std::vector<uint8_t> dram;

 public:
  // 构造函数，初始化寄存器、程序计数器和内存
  Cpu(const std::vector<uint8_t>& code) : pc(0), dram(DRAM_SIZE, 0) {
    // 初始化所有寄存器为0
    regs.fill(0);
    // 设置栈指针寄存器的初始值为内存的最后一个字节地址
    regs[2] = DRAM_SIZE - 1;
    // 将代码加载到内存的起始位置
    std::copy(code.begin(), code.end(), dram.begin());
  }

  // 打印所有寄存器的值
  void printRegs() const {
    for (size_t i = 0; i < regs.size(); ++i) {
      std::cout << "reg[" << i << "] = " << regs[i] << std::endl;
    }
  }

  // 打印内存前16个字节的值
  void printMem() const {
    std::cout << "Memory (first 16 bytes): ";
    for (size_t i = 0; i < 16; ++i) {
      std::cout << static_cast<int>(dram[i]) << " ";
    }
    std::cout << std::endl;
  }

  // 获取指定索引处的寄存器值
  uint64_t getReg(size_t index) const { return regs.at(index); }

  // 获取指定索引处的内存值
  uint8_t getMem(size_t index) const { return dram.at(index); }
};

#endif  // CPU_H
