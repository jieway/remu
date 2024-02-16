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

namespace crvemu {
std::optional<uint64_t> Cpu::load(uint64_t addr, uint64_t size) {
  auto val = bus.load(addr, size);
  if (val.has_value()) {
    LOG(INFO, "Load successful. Value: ", std::hex, val.value());
    return val;
  }

  LOG(WARNING, "Load failed. Invalid address: ", std::hex, addr);
  return std::nullopt;
}

bool Cpu::store(uint64_t addr, uint64_t size, uint64_t value) {
  LOG(INFO, "Storing value ", value, " at address ", std::hex, addr, " with size ", size, " bytes.");

  // Assuming bus.store returns a boolean indicating success
  bool result = bus.store(addr, size, value);

  if (result) {
    LOG(INFO, "Store successful.");
  } else {
    LOG(WARNING, "Store failed.");
  }

  return result;
}

std::optional<uint32_t> Cpu::fetch() {
  auto inst = bus.load(pc, 32);
  if (inst.has_value()) {
    LOG(INFO, "Instruction fetched: ", std::hex, inst.value(), std::dec);
    return inst.value();
  }
  LOG(WARNING, "Fetch failed. Invalid PC: ", std::hex, pc);
  return std::nullopt;
}

std::optional<uint64_t>  Cpu::execute(uint32_t inst) {
  auto exe = InstructionExecutor::execute(*this, inst);
  if (exe.has_value()) {
    LOG(INFO, "Execution successful. Result: ", std::hex, exe.value());
    return exe;
  }
  LOG(WARNING, "Execution failed.");
  return std::nullopt;
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
}