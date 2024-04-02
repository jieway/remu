//
// Created by Jie Wei on 2024/2/4.
//

// Cpu.cpp
#include <iostream>
#include <iomanip> // 用于格式化输出
#include <optional>
#include "cup.h"
#include "instructions.h"
#include "log.h"

namespace cemu {

std::optional<uint64_t> Cpu::load(uint64_t addr, uint64_t size) {
  return bus.load(addr, size);
}

bool Cpu::store(uint64_t addr, uint64_t size, uint64_t value) {
  return bus.store(addr, size, value);
}

std::optional<uint32_t> Cpu::fetch() {
  auto inst = bus.load(pc, 32);
  if (inst.has_value()) {
    LOG(INFO, "Instruction fetched: ", std::hex, inst.value(), std::dec);
    return inst.value();
  }
  throw Exception(ExceptionType::InstructionAccessFault, pc);
}

std::optional<uint64_t>  Cpu::execute(uint32_t inst) {
  auto exe = InstructionExecutor::execute(*this, inst);
  if (exe.has_value()) {
    LOG(INFO, "Execution successful. Result: ", std::hex, exe.value());
    return exe;
  }
  throw Exception(ExceptionType::IllegalInstruction, pc);
}

void Cpu::dump_registers() {
  LOG(INFO, "Dumping register state:");

  std::cout << std::setw(80) << std::setfill('-') << "" << std::endl; // 打印分隔线
  std::cout << std::setfill(' '); // 重置填充字符
  for (size_t i = 0; i < 32; i += 4) {
    std::cout << std::setw(4) << "x" << i << "(" << RVABI[i] << ") = " << std::hex << std::setw(16) << std::setfill('0') << regs[i] << " "
              << std::setw(4) << "x" << i + 1 << "(" << RVABI[i + 1] << ") = " << std::setw(16) << regs[i + 1] << " "
              << std::setw(4) << "x" << i + 2 << "(" << RVABI[i + 2] << ") = " << std::setw(16) << regs[i + 2] << " "
              << std::setw(4) << "x" << i + 3 << "(" << RVABI[i + 3] << ") = " << std::setw(16) << regs[i + 3] << std::endl;
  }
}

void Cpu::dump_pc() const {
  LOG(INFO, "Dumping PC register state:");
  LOG(INFO, "PC = 0x", std::hex, pc, std::dec);
}

std::optional<uint64_t> Cpu::getRegValueByName(const std::string& name) {
  // 尝试在寄存器中查找
  auto it = std::find(RVABI.begin(), RVABI.end(), name);
  if (it != RVABI.end()) {
    int index = std::distance(RVABI.begin(), it);
    return regs[index];
  }

  // 创建一个映射，将寄存器的名称映射到对应的加载函数
  std::unordered_map<std::string, std::function<std::optional<uint64_t>()>> csr_map = {
    {"mhartid", [this]() { return csr.load(MHARTID); }},
    {"mstatus", [this]() { return csr.load(MSTATUS); }},
    {"mtvec", [this]() { return csr.load(MTVEC); }},
    {"mepc", [this]() { return csr.load(MEPC); }},
    {"mcause", [this]() { return csr.load(MCAUSE); }},
    {"mtval", [this]() { return csr.load(MTVAL); }},
    {"medeleg", [this]() { return csr.load(MEDELEG); }},
    {"mscratch", [this]() { return csr.load(MSCRATCH); }},
    {"MIP", [this]() { return csr.load(MIP); }},
    {"mcounteren", [this]() { return csr.load(MCOUNTEREN); }},
    {"sstatus", [this]() { return csr.load(SSTATUS); }},
    {"stvec", [this]() { return csr.load(STVEC); }},
    {"sepc", [this]() { return csr.load(SEPC); }},
    {"scause", [this]() { return csr.load(SCAUSE); }},
    {"stval", [this]() { return csr.load(STVAL); }},
    {"sscratch", [this]() { return csr.load(SSCRATCH); }},
    {"SIP", [this]() { return csr.load(SIP); }},
    {"SATP", [this]() { return csr.load(SATP); }},
  };

  // 尝试在映射中查找
  auto map_it = csr_map.find(name);
  if (map_it != csr_map.end()) {
    return map_it->second();
  }

  // 如果在寄存器和CSR寄存器中都找不到，返回std::nullopt
  LOG(WARNING, "Invalid name: ", name);
  return std::nullopt;
}

void Cpu::handle_exception(const Exception& e) {
  uint64_t pc = this->pc;   // 保存当前 PC 寄存器的值
  Mode mode = this->mode;   // 保存当前模式
  uint64_t cause = static_cast<uint64_t>(e.getType()); // 获取异常原因

  // 是否在 S 模式下陷入
  bool trap_in_s_mode = mode <= Supervisor && csr.is_medelegated(cause);
  uint64_t STATUS, TVEC, CAUSE, TVAL, EPC, MASK_PIE, pie_i, MASK_IE, ie_i, MASK_PP, pp_i;

  // 根据是否在 S 模式下陷入，设置不同的寄存器
  if (trap_in_s_mode) {
    this->mode = Supervisor;
    STATUS = SSTATUS;
    TVEC = STVEC;
    CAUSE = SCAUSE;
    TVAL = STVAL;
    EPC = SEPC;
    MASK_PIE = MASK_SPIE;
    pie_i = 5;
    MASK_IE = MASK_SIE;
    ie_i = 1;
    MASK_PP = MASK_SPP;
    pp_i = 8;
  } else {
    this->mode = Machine;
    STATUS = MSTATUS;
    TVEC = MTVEC;
    CAUSE = MCAUSE;
    TVAL = MTVAL;
    EPC = MEPC;
    MASK_PIE = MASK_MPIE;
    pie_i = 7;
    MASK_IE = MASK_MIE;
    ie_i = 3;
    MASK_PP = MASK_MPP;
    pp_i = 11;
  }

  // 将程序计数器（PC）设置为异常向量表（TVEC）寄存器的值。
  // 这是因为在 RISC-V 架构中，当发生异常时，CPU 会跳转到
  // TVEC 寄存器指定的地址开始执行异常处理程序。
  this->pc = csr.load(TVEC) & ~0b11;

  // 将当前的 PC 寄存器的值保存到异常程序计数器（EPC）寄存器中。
  // 这是为了在异常处理程序执行完毕后，可以通过 EPC 寄存器恢复
  // 原来的 PC 值，继续执行异常发生前的程序。
  csr.store(EPC, pc);

  //  这两行代码将异常的原因和值保存到 CAUSE 和 TVAL 寄存器中。
  //  这是为了在异常处理程序中可以获取到这些信息，以便进行相应的处理
  csr.store(CAUSE, cause);
  csr.store(TVAL, e.getValue());

  //  读取当前的状态寄存器（STATUS）的值。
  uint64_t status = csr.load(STATUS);
  //  获取当前的中断使能（IE）位的值。
  uint64_t ie = (status & MASK_IE) >> ie_i;
  //  将当前的中断使能（IE）位的值保存到上一次中断使能（PIE）位。
  status = (status & ~MASK_PIE) | (ie << pie_i);
  // 将当前的中断使能（IE）位设置为 0，即禁用中断。
  status &= ~MASK_IE;
  // 将当前的特权级（PP）位设置为当前模式。
  status = (status & ~MASK_PP) | (mode << pp_i);
  // 将状态寄存器（STATUS）的值保存回 CSR 中。
  csr.store(STATUS, status);

}

}