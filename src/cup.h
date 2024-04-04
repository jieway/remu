//
// Created by Jie Wei on 2024/2/4.
//

// Cpu.h
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "bus.h"
#include "csr.h"
#include "exception.h"
#include "interrupt.h"

namespace cemu {

class Cpu {
public:

  // PC 寄存器包含下一条指令的内存地址
  uint64_t pc;

  // RISC-V 有 32 个寄存器
  std::array<uint64_t, 32> regs{};

  Bus bus;
  Mode mode;

  // 控制和状态寄存器。RISC-V ISA为最多4096个CSR预留了一个12位的编码空间（csr[11:0]）。
  Csr csr;

  Cpu(const std::vector<uint8_t>& code)
      : pc(DRAM_BASE),
        bus(code),
        csr()  // 初始化 Csr
  {
      regs.fill(0); // 初始化寄存器为0
      regs[2] = DRAM_END; // 设置堆栈指针寄存器的初始值
      mode = Machine;
  }

  std::optional<uint64_t> load(uint64_t addr, uint64_t size); // 从内存中读取数据
  bool store(uint64_t addr, uint64_t size, uint64_t value); // 将数据写入内存
  std::optional<uint32_t> fetch();  // 从内存中获取指令

  [[nodiscard]] inline uint64_t update_pc() const {
    return pc + 4;
  }

  std::optional<uint64_t> execute(uint32_t inst); // 执行指令

  void dump_registers();   // 打印所有寄存器的值
  void dump_pc() const;    // 打印 PC 寄存器的值

  std::optional<uint64_t> getRegValueByName(const std::string& name); // 通过名称获取寄存器的值

  void handle_exception(const Exception& e);          // 处理异常

  void handle_interrupt(Interrupt& interrupt);        // 处理中断

  std::optional<Interrupt> check_pending_interrupt(); // 检查是否有中断

private:
  // 在类外初始化静态成员
  const std::array<std::string, 32> RVABI = {
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
  };

};

}


