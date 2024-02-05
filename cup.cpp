//
// Created by Jie Wei on 2024/2/4.
//

// Cpu.cpp
#include <iostream>
#include <fstream>
#include <iomanip> // 用于格式化输出
#include <optional>
#include "cup.h"
#include "exception.h"
#include "instructions.h"

uint64_t Cpu::load(uint64_t addr, uint64_t size) {
    try {
        return bus.load(addr, size);
    } catch (const Exception& e) {
        std::cerr << "Exception load: " << e << std::endl;
    }
}

void Cpu::store(uint64_t addr, uint64_t size, uint64_t value) {
    try {
        bus.store(addr, size, value);
    } catch (const Exception& e) {
        std::cerr << "Exception store: " << e << std::endl;
    }
}

uint32_t Cpu::fetch() {
    try {
        bus.load(pc, 32);
    } catch (const Exception& e) {
        std::cerr << "Exception fetch: " << e << std::endl;
    }
}

std::optional<uint64_t>  Cpu::execute(uint32_t inst) {
    try {
        uint32_t opcode = inst & 0x7f;
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        uint32_t rs2 = (inst >> 20) & 0x1f;
        uint32_t funct3 = (inst >> 12) & 0x7;
        uint32_t funct7 = (inst >> 25) & 0x7f;
        int64_t immediate = static_cast<int64_t>(inst & 0xfff00000) >> 20;

        return InstructionExecutor::execute(*this, opcode, rd, rs1, rs2, funct3, funct7, immediate);
    } catch (const Exception& e) {
        std::cerr << "Exception execute : " << e << std::endl;
        return std::nullopt;  // 使用 std::optional 表示异常
    }
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