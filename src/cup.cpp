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

  // 尝试在CSR寄存器中查找
  if (name == "mhartid") {
    return csr.load(MHARTID);
  } else if (name == "mstatus") {
    return csr.load(MSTATUS);
  } else if (name == "mtvec") {
    return csr.load(MTVEC);
  } else if (name == "mepc") {
    return csr.load(MEPC);
  } else if (name == "mcause") {
    return csr.load(MCAUSE);
  } else if (name == "mtval") {
    return csr.load(MTVAL);
  } else if (name == "medeleg") {
    return csr.load(MEDELEG);
  } else if (name == "mscratch") {
    return csr.load(MSCRATCH);
  } else if (name == "MIP") {
    return csr.load(MIP);
  } else if (name == "mcounteren") {
    return csr.load(MCOUNTEREN);
  } else if (name == "sstatus") {
    return csr.load(SSTATUS);
  } else if (name == "stvec") {
    return csr.load(STVEC);
  } else if (name == "sepc") {
    return csr.load(SEPC);
  } else if (name == "scause") {
    return csr.load(SCAUSE);
  } else if (name == "stval") {
    return csr.load(STVAL);
  } else if (name == "sscratch") {
    return csr.load(SSCRATCH);
  } else if (name == "SIP") {
    return csr.load(SIP);
  } else if (name == "SATP") {
    return csr.load(SATP);
  }

  // 如果在寄存器和CSR寄存器中都找不到，返回std::nullopt
  LOG(WARNING, "Invalid name: ", name);
  return std::nullopt;
}
}