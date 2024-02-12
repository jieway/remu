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
    LOG(INFO, "Load successful. Value: ", val.value());
    return val;
  }

  LOG(ERROR, "Load failed. Invalid address: ", addr);
  return std::nullopt;
}

bool Cpu::store(uint64_t addr, uint64_t size, uint64_t value) {
  LOG(INFO, "Storing value ", value, " at address ", addr, " with size ", size, " bytes.");

  // Assuming bus.store returns a boolean indicating success
  bool result = bus.store(addr, size, value);

  if (result) {
    LOG(INFO, "Store successful.");
  } else {
    LOG(ERROR, "Store failed.");
  }

  return result;
}

std::optional<uint32_t> Cpu::fetch() {
  auto inst = bus.load(pc, 32);
  if (inst.has_value()) {
    LOG(INFO, "Instruction fetched: ", std::hex, inst.value(), std::dec);
    return inst.value();
  }
  LOG(ERROR, "Fetch failed. Invalid PC: ", pc);
  return std::nullopt;
}

std::optional<uint64_t>  Cpu::execute(uint32_t inst) {
  auto exe = InstructionExecutor::execute(*this, inst);
  if (exe.has_value()) {
    LOG(INFO, "Execution successful. Result: ", exe.value());
    return exe;
  }
  LOG(ERROR, "Execution failed.");
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
}