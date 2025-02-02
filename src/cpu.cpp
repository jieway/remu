#include "cpu.h"

#include <format>  // 📝 使用 std::format 格式化字符串，便于输出调试信息
#include <iomanip>  // 📏 控制输出格式
#include <sstream>  // 🛠️ 使用字符串流拼接字符串

#include "logger.h"  // 🔧 日志库，用于调试和记录信息

// 🏗️ 构造函数：使用 RISC-V 二进制程序代码初始化 CPU 对象
// 参数说明：
//   code - 包含 RISC-V 程序二进制代码的内存缓冲区
Cpu::Cpu(std::span<const uint8_t> code)
    : mem(code.begin(), code.end())  // 📥 将代码复制到内部内存中存储
{
  // 🚀 初始化寄存器状态
  regs[2] =
      mem.size() - 8;  // 📌 初始化栈指针 (x2)：指向内存末尾, 并保证8字节对齐
  regs[0] = 0;  // 🔒 x0 寄存器硬编码为 0，始终保持不变
}

// 🎯 取指函数：从内存中读取当前指令，并正确处理小端序
// 返回值：32 位 RISC-V 指令编码
uint32_t Cpu::fetch() {
  uint32_t instr = 0;
  // 🔄 按小端序读取指令：低字节在前，高字节在后
  instr |= static_cast<uint32_t>(mem[pc + 3]) << 24;  // 📦 第4字节（最高位）
  instr |= static_cast<uint32_t>(mem[pc + 2]) << 16;  // 📦 第3字节
  instr |= static_cast<uint32_t>(mem[pc + 1]) << 8;   // 📦 第2字节
  instr |= static_cast<uint32_t>(mem[pc]);  // 📦 第1字节（最低位）
  return instr;
}

// ⚡ 执行指令：解码并执行一条 RISC-V 指令
// 参数说明：
//   instr - 需要执行的 32 位指令编码
void Cpu::execute(uint32_t instr) {
  // 🔍 解码指令，将指令拆分为各个字段，便于识别与执行
  auto fields = decode_instruction(instr);

  // 🧩 利用 std::format 格式化输出详细的指令解码信息
  auto instr_info = std::format(
      "Instruction Decode Info:\n"
      "Raw bytes: {0:#x} {1:#x} {2:#x} {3:#x}\n"  // 原始字节信息
      "Full instruction: 0x{4:#x}\n"              // 整个指令编码
      "opcode: 0x{5:#x}\n"                        // 操作码字段
      "rd: {6}\n"                                 // 目的寄存器编号
      "rs1: {7}\n"                                // 源寄存器1编号
      "funct3: 0x{8:#x}\n"                        // 功能字段3
      "funct7: 0x{9:#x}\n"                        // 功能字段7
      "i_imm: {10}",                              // 立即数
      static_cast<int>(instr & 0xFF),             // 字节1
      static_cast<int>((instr >> 8) & 0xFF),      // 字节2
      static_cast<int>((instr >> 16) & 0xFF),     // 字节3
      static_cast<int>((instr >> 24) & 0xFF),     // 字节4
      instr, static_cast<int>(fields.opcode), static_cast<int>(fields.rd),
      static_cast<int>(fields.rs1), static_cast<int>(fields.funct3),
      static_cast<int>(fields.funct7), fields.i_imm);
  Logger::debug(instr_info);  // 📣 输出解码信息到调试日志

  // 🎮 根据解码后的操作码执行指令
  switch (fields.opcode) {
    // ➕ ADDI 指令：执行 x[rd] = x[rs1] + i_imm
    [[likely]] case OpCode::ADDI:
      Logger::info(std::format("Executing ADDI instruction: x{} = x{} + {}",
                               static_cast<int>(fields.rd),
                               static_cast<int>(fields.rs1), fields.i_imm));
      regs[fields.rd] = regs[fields.rs1] + fields.i_imm;
      break;

    // ➕ ADD 指令：当 funct3 和 funct7 均为0时，执行 x[rd] = x[rs1] + x[rs2]
    [[likely]] case OpCode::ADD:
      if (fields.funct3 == 0x0 && fields.funct7 == 0x0) {
        Logger::info(std::format("Executing ADD instruction: x{} = x{} + x{}",
                                 static_cast<int>(fields.rd),
                                 static_cast<int>(fields.rs1),
                                 static_cast<int>(fields.rs2)));
        regs[fields.rd] = regs[fields.rs1] + regs[fields.rs2];
      }
      // 当 funct3=0x0 且 funct7=0x20 时执行减法
      else if (fields.funct3 == 0x0 && fields.funct7 == 0x20) {
        Logger::info(std::format("Executing SUB instruction: x{} = x{} - x{}",
                                 static_cast<int>(fields.rd),
                                 static_cast<int>(fields.rs1),
                                 static_cast<int>(fields.rs2)));
        regs[fields.rd] = regs[fields.rs1] - regs[fields.rs2];
      } else {
        Logger::warn(std::format(
            "Unknown variation for ADD opcode! funct3=0x{:x}, funct7=0x{:x}",
            static_cast<int>(fields.funct3), static_cast<int>(fields.funct7)));
      }
      break;

    default:  // ❓ 未知或未实现的指令
      Logger::warn(std::format("Unknown instruction! opcode: 0x{0:#x}",
                               static_cast<int>(fields.opcode)));
  }

  // 🔄 更新程序计数器，将 pc 前进至下一条指令位置
  pc += 4;
  regs[0] = 0;  // 🔒 确保 x0 寄存器始终保持为 0
}

// 📊 调试函数：输出当前 CPU 状态（程序计数器及各寄存器的值）
// 利用 std::format 生成格式化字符串输出
void Cpu::debug() {
  Logger::debug("\n=== CPU State ===");
  Logger::debug(std::format("PC: 0x{0:#x}", pc));

  // 🔠 定义寄存器名称数组，便于调试时直观查看各寄存器的作用
  const std::array reg_names = {
      "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0/fp", "s1", "a0",
      "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3",    "s4", "s5",
      "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5",    "t6"};

  // 🗒️ 循环遍历寄存器数组，每行打印 4 个寄存器的状态，增强可读性
  for (size_t i = 0; i < regs.size(); i += 4) {
    std::string line;
    for (size_t j = 0; j < 4 && (i + j) < regs.size(); ++j) {
      line += std::format("x{}({}): 0x{:#x}\t", i + j, reg_names[i + j],
                          regs[i + j]);
    }
    Logger::debug(line);
  }
  Logger::debug("================");
}

// 🔍 指令解码函数：将 32 位指令拆分为各个字段，方便后续操作
// 返回值：InstructionFields 结构体，包含 opcode, rd, rs1, rs2, funct3, funct7
// 和 i_imm 字段
InstructionFields Cpu::decode_instruction(uint32_t instr) {
  return InstructionFields{
      .opcode = static_cast<OpCode>(instr & 0x7F),  // 📟 取最低 7 位为操作码
      .rd = static_cast<uint8_t>((instr >> 7) &
                                 0x1F),  // 🎯 获取目标寄存器编号（5位）
      .rs1 = static_cast<uint8_t>((instr >> 15) &
                                  0x1F),  // 🔄 获取第一个源寄存器编号（5位）
      .rs2 = static_cast<uint8_t>((instr >> 20) &
                                  0x1F),  // 🔄 获取第二个源寄存器编号（5位）
      .funct3 =
          static_cast<uint8_t>((instr >> 12) & 0x7),  // 🧩 获取功能字段3（3位）
      .funct7 = static_cast<uint8_t>((instr >> 25) &
                                     0x7F),  // 🧩 获取功能字段7（7位）
      .i_imm = static_cast<int32_t>(instr & 0xFFF00000) >>
               20  // 💡 提取立即数，并通过右移实现符号扩展
  };
}