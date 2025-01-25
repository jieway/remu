#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

// 🖥️ CPU模拟器结构体：模拟RISC架构处理器核心组件
struct Cpu {
  std::array<uint64_t, 32> regs{};  // 🧠 32个通用寄存器（x0-x31）
  uint64_t pc = 0;                  // 📍 程序计数器
  std::vector<uint8_t> mem;         // 💾 模拟内存

  // 🏗️ 构造函数：用程序代码初始化CPU
  Cpu(const std::vector<uint8_t>& code) : mem(code) {
    // 🚀 初始化寄存器状态
    regs[2] = mem.size() - 8;  // 📌 栈指针8字节对齐（x2寄存器）
    regs[0] = 0;               // 🔒 硬编码x0寄存器为0
  }

  // 🚀 取指：从内存中读取当前指令
  uint32_t fetch() {
    // ⚠️ 简单内存访问（实际需要边界检查）
    return *(uint32_t*)(&mem[pc]);
  }

  // 🎯 执行指令的核心方法
  void execute(uint32_t instr) {
    // 🔍 解析指令字段
    uint8_t opcode = instr & 0x7F;       // 🎛️ 操作码（低7位）
    uint8_t rd = (instr >> 7) & 0x1F;    // 📥 目标寄存器编号
    uint8_t rs1 = (instr >> 15) & 0x1F;  // 📤 源寄存器1编号
    int64_t imm =
        (int64_t)(int32_t)(instr & 0xFFF00000) >> 20;  // 📡 符号扩展立即数

    // 🛠️ ADDI指令处理（I-type）
    if (opcode == 0x13) {          // 🔖 操作码0x13对应ADDI
      regs[rd] = regs[rs1] + imm;  // ➕ 执行加法操作
    }

    pc += 4;      // ⏩ 默认指令步长（RISC-V为32位定长）
    regs[0] = 0;  // 🔐 强制归零寄存器保持为0
  }

  // 📊 调试输出：显示关键寄存器状态
  void debug() {
    std::cout << "PC: 0x" << std::hex << pc << " | x1: 0x" << regs[1]
              << " | x2(sp): 0x" << regs[2] << std::endl;
  }
};

int main() {
  // 📦 示例程序：ADDI x1, x0, 0x42
  // 🔧 机器码：0x04200093（小端存储）
  std::vector<uint8_t> code(1024);
  code[0] = 0x93;  // 🌀 最低字节（0x93）
  code[1] = 0x00;  // 🌀 funct3和寄存器部分
  code[2] = 0x20;  // 🌀 立即数低位
  code[3] = 0x04;  // 🌀 立即数高位（小端存储）

  // 🔌 初始化CPU实例
  Cpu cpu(code);

  // 🎮 模拟器运行周期
  // 🔄 取指-执行循环（此处简化为单步执行）
  uint32_t instr = cpu.fetch();
  cpu.execute(instr);

  // 📝 验证执行结果
  cpu.debug();  // ✅ 预期输出：x1 = 0x42

  // ======================
  // 🧪 自动化验证代码
  // ======================
  std::cout << "\n=== 🔍 Result Validation ===" << std::endl;
  bool all_pass = true;

  // 🔄 验证点1：x1寄存器值
  const uint64_t expected_x1 = 0x42;
  if (cpu.regs[1] == expected_x1) {
    std::cout << "✅ x1 register check passed (0x" << std::hex << expected_x1
              << ")" << std::endl;
  } else {
    std::cout << "❌ x1 register mismatch | Expected: 0x" << expected_x1
              << " | Actual: 0x" << cpu.regs[1] << std::endl;
    all_pass = false;
  }

  // 🔄 验证点2：程序计数器PC
  const uint64_t expected_pc = 0x4;
  if (cpu.pc == expected_pc) {
    std::cout << "✅ PC check passed (0x" << expected_pc << ")" << std::endl;
  } else {
    std::cout << "❌ PC mismatch | Expected: 0x" << expected_pc
              << " | Actual: 0x" << cpu.pc << std::endl;
    all_pass = false;
  }

  // 🔄 验证点3：栈指针x2
  const uint64_t expected_sp = 0x3f8;  // 1024-8=1016=0x3F8
  if (cpu.regs[2] == expected_sp) {
    std::cout << "✅ Stack pointer check passed (0x" << expected_sp << ")"
              << std::endl;
  } else {
    std::cout << "❌ Stack pointer mismatch | Expected: 0x" << expected_sp
              << " | Actual: 0x" << cpu.regs[2] << std::endl;
    all_pass = false;
  }

  // 🏁 最终测试结果
  std::cout << "\nTest Result: ";
  if (all_pass) {
    std::cout << "🎉 All checks passed!";
  } else {
    std::cout << "⚠️ Some checks failed!";
  }
  std::cout << std::endl;

  return all_pass ? 0 : 1;
}