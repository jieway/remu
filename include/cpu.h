#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <iomanip>  // 用于格式化输出
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

  // 获取当前pc指向的指令
  uint32_t fetch() const {
    size_t index = static_cast<size_t>(pc);
    uint32_t inst = static_cast<uint32_t>(dram[index]) |
                    (static_cast<uint32_t>(dram[index + 1]) << 8) |
                    (static_cast<uint32_t>(dram[index + 2]) << 16) |
                    (static_cast<uint32_t>(dram[index + 3]) << 24);
    return inst;
  }

  // 执行指令
  void execute(uint32_t inst) {
    uint32_t opcode = inst & 0x7f;
    uint32_t rd = (inst >> 7) & 0x1f;
    uint32_t rs1 = (inst >> 15) & 0x1f;
    uint32_t rs2 = (inst >> 20) & 0x1f;
    uint32_t funct3 = (inst >> 12) & 0x7;
    uint32_t funct7 = (inst >> 25) & 0x7f;

    std::cout << "opcode: " << std::hex << opcode << " rd: " << rd
              << " rs1: " << rs1 << " rs2: " << rs2 << std::endl;
    std::cout << "regs[rs1]: " << regs[rs1] << " regs[rs2]: " << regs[rs2]
              << std::endl;

    // x0 is hardwired zero
    regs[0] = 0;

    switch (opcode) {
      case 0x13: {  // addi
        int64_t imm = static_cast<int32_t>(inst & 0xfff00000) >> 20;
        regs[rd] = regs[rs1] + imm;
        break;
      }
      case 0x33: {  // add
        regs[rd] = regs[rs1] + regs[rs2];
        break;
      }
      default:
        std::cerr << "Invalid opcode: " << std::hex << opcode << std::endl;
        break;
    }
  }

  // 打印所有寄存器的值
  void printRegs() const {
    std::cout << std::setw(80) << std::setfill('-') << ""
              << std::endl;          // 打印分隔线
    std::cout << std::setfill(' ');  // 重置填充字符
    for (size_t i = 0; i < regs.size(); i += 4) {
      std::cout << std::setw(4) << "x" << i << " = " << std::hex
                << std::setw(16) << std::setfill('0') << regs[i] << " "
                << std::setw(4) << "x" << i + 1 << " = " << std::setw(16)
                << regs[i + 1] << " " << std::setw(4) << "x" << i + 2 << " = "
                << std::setw(16) << regs[i + 2] << " " << std::setw(4) << "x"
                << i + 3 << " = " << std::setw(16) << regs[i + 3] << std::endl;
    }
  }

  // 打印内存前16个字节的值
  void printMem() const {
    std::cout << "Memory (first 16 bytes): ";
    for (size_t i = 0; i < 16; ++i) {
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(dram[i]) << " ";
    }
    std::cout << std::endl;
  }

  // 获取指定索引处的寄存器值
  uint64_t getReg(size_t index) const { return regs.at(index); }

  // 获取指定索引处的内存值
  uint8_t getMem(size_t index) const { return dram.at(index); }

  // 修改指定寄存器的值
  void setReg(size_t index, uint64_t value) {
    if (index != 0) regs.at(index) = value;
  }

  // 修改指定内存地址的值
  void setMem(size_t index, uint8_t value) { dram.at(index) = value; }
};

#endif  // CPU_H
