#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

// 🎯 指令操作码枚举：定义支持的RISC-V指令类型
enum class OpCode : uint8_t {
  ADDI = 0x13,  // ➕ 立即数加法指令
  ADD = 0x33,   // ➕ 寄存器加法指令
  // 🔄 后续可以轻松添加更多指令
};

// 🔍 指令字段结构体：存储解码后的指令各个字段
struct InstructionFields {
  OpCode opcode;   // 📝 操作码：标识指令类型
  uint8_t rd;      // 📥 目标寄存器
  uint8_t rs1;     // 📤 源寄存器1
  uint8_t rs2;     // 📤 源寄存器2
  uint8_t funct3;  // 🔧 功能码3：进一步细分指令功能
  uint8_t funct7;  // 🔧 功能码7：用于区分相同操作码的不同指令
  int32_t i_imm;  // 💫 立即数：指令中的常量值
};

// 🖥️ CPU模拟器结构体：模拟RISC-V处理器的核心组件
struct Cpu {
  std::array<uint64_t, 32> regs{};  // 🧠 32个通用寄存器（x0-x31）
  uint64_t pc = 0;  // 📍 程序计数器：指向当前执行的指令
  std::vector<uint8_t> mem;  // 💾 模拟内存：存储程序和数据

  // 🏗️ 构造函数：初始化CPU状态和加载程序
  explicit Cpu(const std::vector<uint8_t>& code);

  // 🎯 取指阶段：从内存中读取当前指令
  uint32_t fetch();

  // ⚡ 执行阶段：处理当前指令并更新CPU状态
  void execute(uint32_t instr);

  // 🔍 调试功能：输出CPU当前状态信息
  void debug();

 private:
  // 🔄 指令解码：将32位指令解析为各个功能字段
  static InstructionFields decode_instruction(uint32_t instr);
};

#endif  // CPU_H