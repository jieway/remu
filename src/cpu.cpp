#include "cpu.h"

#include <iomanip>

// 🏗️ 构造函数：用程序代码初始化CPU
// 💡 参数code: 包含RISC-V程序二进制代码的内存缓冲区
Cpu::Cpu(const std::vector<uint8_t>& code) : mem(code) {
  // 🚀 初始化寄存器状态
  regs[2] = mem.size() - 8;  // 📌 栈指针8字节对齐（x2寄存器）
  regs[0] = 0;               // 🔒 硬编码x0寄存器为0
}

// 🎯 取指：从内存中读取当前指令
// 💫 返回值: 32位的RISC-V指令
uint32_t Cpu::fetch() {
  // 🔄 正确处理小端序（little-endian）读取
  uint32_t instr = 0;
  instr |= (uint32_t)mem[pc + 3] << 24;  // 最高字节
  instr |= (uint32_t)mem[pc + 2] << 16;
  instr |= (uint32_t)mem[pc + 1] << 8;
  instr |= (uint32_t)mem[pc];  // 最低字节
  return instr;
}

// ⚡ 执行指令的核心方法
// 💡 参数instr: 32位的RISC-V指令
void Cpu::execute(uint32_t instr) {
  // 🔍 解码指令字段
  auto fields = decode_instruction(instr);

  // 🐛 打印调试信息
  std::cout << "\nInstruction Decode Info:" << std::hex
            << "\nRaw bytes: " << (int)(instr & 0xFF) << " "
            << (int)((instr >> 8) & 0xFF) << " " << (int)((instr >> 16) & 0xFF)
            << " " << (int)((instr >> 24) & 0xFF) << "\nFull instruction: 0x"
            << instr << "\nopcode: 0x" << static_cast<int>(fields.opcode)
            << "\nrd: " << std::dec << (int)fields.rd
            << "\nrs1: " << (int)fields.rs1 << "\nfunct3: 0x" << std::hex
            << (int)fields.funct3 << "\nfunct7: 0x" << (int)fields.funct7
            << "\ni_imm: " << std::dec << fields.i_imm << std::endl;

  // 🎮 根据操作码执行相应指令
  switch (fields.opcode) {
    case OpCode::ADDI:  // ➕ 立即数加法指令
      std::cout << "Executing ADDI instruction: x" << std::dec << (int)fields.rd
                << " = x" << (int)fields.rs1 << " + " << fields.i_imm
                << std::endl;
      regs[fields.rd] = regs[fields.rs1] + fields.i_imm;
      break;

    case OpCode::ADD:  // ➕ 寄存器加法指令
      if (fields.funct3 == 0x0 && fields.funct7 == 0x0) {
        std::cout << "Executing ADD instruction: x" << std::dec
                  << (int)fields.rd << " = x" << (int)fields.rs1 << " + x"
                  << (int)fields.rs2 << std::endl;
        regs[fields.rd] = regs[fields.rs1] + regs[fields.rs2];
      }
      break;

    default:  // ❓ 未知指令处理
      std::cout << "Unknown instruction! opcode: 0x" << std::hex
                << static_cast<int>(fields.opcode) << std::endl;
  }

  pc += 4;      // 🔄 更新程序计数器，指向下一条指令
  regs[0] = 0;  // 🔒 确保x0始终为0
}

// 📊 调试输出：显示关键寄存器状态
void Cpu::debug() {
  std::cout << "\n=== CPU State ===\n";
  std::cout << "PC: 0x" << std::hex << pc << std::endl;

  // 📝 打印所有寄存器的值，每行打印4个寄存器
  const char* reg_names[] = {
      "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0/fp", "s1", "a0",
      "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3",    "s4", "s5",
      "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5",    "t6"};

  for (int i = 0; i < 32; i += 4) {
    std::cout << std::hex;
    for (int j = 0; j < 4 && (i + j) < 32; j++) {
      std::cout << "x" << std::dec << (i + j) << "(" << reg_names[i + j]
                << "): 0x" << std::hex << regs[i + j] << "\t";
    }
    std::cout << std::endl;
  }
  std::cout << "================\n";
}

// 🔍 解码指令：将32位指令解析为各个字段
// 💫 返回值: 包含所有指令字段的结构体
InstructionFields Cpu::decode_instruction(uint32_t instr) {
  return InstructionFields{
      .opcode = static_cast<OpCode>(instr & 0x7F),
      .rd = static_cast<uint8_t>((instr >> 7) & 0x1F),
      .rs1 = static_cast<uint8_t>((instr >> 15) & 0x1F),
      .rs2 = static_cast<uint8_t>((instr >> 20) & 0x1F),
      .funct3 = static_cast<uint8_t>((instr >> 12) & 0x7),
      .funct7 = static_cast<uint8_t>((instr >> 25) & 0x7F),
      .i_imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20};
}