//
// Created by Jie Wei on 2024/2/4.
//

// Cpu.cpp
#include <iostream>
#include <iomanip> // 用于格式化输出
#include <optional>
#include "cup.h"
#include "exception.h"
#include "instructions.h"

namespace crvemu {
std::optional<uint64_t> Cpu::load(uint64_t addr, uint64_t size) {
  auto val = bus.load(addr, size);
  if (val.has_value()) {
    return val;
  }
  return std::nullopt;
}

bool Cpu::store(uint64_t addr, uint64_t size, uint64_t value) {
  return bus.store(addr, size, value);
}

std::optional<uint32_t> Cpu::fetch() {
  auto inst = bus.load(pc, 32);
  if (inst.has_value()) {
    return inst.value();
  }
  return std::nullopt;
}

std::optional<uint64_t>  Cpu::execute(uint32_t inst) {
  auto exe = InstructionExecutor::execute(*this, inst);
  if (exe.has_value()) {
    return exe;
  }
  return std::nullopt;
}

void Cpu::dump_registers() {
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
  std::cout << std::setw(80) << std::setfill('-') << "" << std::endl;
  std::cout << "PC register" << std::endl;
  std::cout << "PC = " << std::hex << pc << std::dec << std::endl;
  std::cout << std::setw(80) << std::setfill('-') << "" << std::endl;
}
}