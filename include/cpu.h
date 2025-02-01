#ifndef CPU_H
#define CPU_H

// 📄 文件说明：
// 本文件定义了一个简单的 RISC-V CPU 模拟器所需的数据结构和接口。
// ➡️ 提供的功能包括：
//    - 支持的指令操作码（OpCode）枚举 🎯
//    - 指令解码后得到的各个字段的数据结构（InstructionFields）📝
//    - 模拟 CPU 结构体（Cpu），包含寄存器、程序计数器、内存及相关操作函数 🖥️

#include <array>
#include <cstdint>
#include <iostream>
#include <span>
#include <vector>

// 🎯 指令操作码枚举：定义 CPU 支持的部分 RISC-V 指令类型
enum class OpCode : uint8_t {
  ADDI = 0x13,  // ➕ 立即数加法指令：将寄存器值和一个常量相加
  ADD = 0x33,  // ➕ 寄存器加法指令：将两个寄存器的值相加
  // 🔄 后续可以在此处添加更多指令类型，便于扩展支持其他 RISC-V 指令
};

// 🔍 指令字段结构体：存储从 32 位指令中解码后的各个字段信息
struct InstructionFields {
  OpCode opcode;  // 📝 操作码：标识具体的指令类型
  uint8_t rd;     // 📥 目标寄存器：存储运算结果的寄存器编号
  uint8_t rs1;  // 📤 源寄存器 1：第一个参与运算的寄存器编号
  uint8_t rs2;  // 📤 源寄存器 2：第二个参与运算的寄存器编号（若无可忽略）
  uint8_t funct3;  // 🔧 功能码3：对指令进行进一步细分，确定具体操作
  uint8_t funct7;  // 🔧 功能码7：在某些指令中用于区分相似操作码的不同指令
  int32_t i_imm;  // 💫 立即数：内嵌在指令中的常量，用于运算时的数值操作
};

// 🖥️ CPU模拟器结构体：模拟一个简化版的 RISC-V 处理器核心组件
struct Cpu {
  std::array<uint64_t, 32> regs{};  // 🧠 32个通用寄存器：分别对应 x0 到 x31
  uint64_t pc = 0;  // 📍 程序计数器（Program Counter）：指向即将执行的指令地址
  std::vector<uint8_t> mem;  // 💾 模拟内存：使用动态数组模拟程序和数据存储

  // 🏗️ 构造函数：初始化 CPU 状态并加载程序代码
  // 参数 code：使用 std::span 来传入只读的程序代码（字节序列）
  explicit Cpu(std::span<const uint8_t> code);

  // 🎯 取指阶段：从内存中按照当前 pc 指定的地址读取 32 位指令
  // 返回值：32 位无符号整型的指令码
  uint32_t fetch();

  // ⚡ 执行阶段：针对取出的指令进行解码和执行
  // 参数 instr：32 位指令码
  // 功能：根据指令内容更新 CPU 内的寄存器、pc 等状态
  void execute(uint32_t instr);

  // 🔍 调试功能：输出当前 CPU 状态的信息，用于调试和验证执行效果
  void debug();

 private:
  // 🔄 指令解码（内部使用）：将一个 32 位指令转换为 InstructionFields
  // 参数 instr：32 位指令码
  // 返回值：包含所有详细信息的 InstructionFields 数据结构
  static InstructionFields decode_instruction(uint32_t instr);
};

#endif  // CPU_H