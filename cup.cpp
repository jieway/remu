//
// Created by Jie Wei on 2024/2/4.
//

// Cpu.cpp
#include <iostream>
#include <fstream>
#include <iomanip> // 用于格式化输出
#include "cup.h"


uint64_t Cpu::load(uint64_t addr, uint64_t size) {
    return bus.load(addr, size);
}

void Cpu::store(uint64_t addr, uint64_t size, uint64_t value) {
    bus.store(addr, size, value);
}

uint32_t Cpu::fetch() {
    return bus.load(pc, 32);
}

uint64_t Cpu::execute(uint32_t inst) {
    uint32_t opcode = inst & 0x7f;
    uint32_t rd = (inst >> 7) & 0x1f;
    uint32_t rs1 = (inst >> 15) & 0x1f;
    uint32_t rs2 = (inst >> 20) & 0x1f;
    uint32_t funct3 = (inst >> 12) & 0x7;
    uint32_t funct7 = (inst >> 25) & 0x7f;

    // x0 is hardwired zero
    regs[0] = 0;

    // execute stage
    switch (opcode) {
        case 0x13: { // addi
            int64_t imm = static_cast<int32_t>(inst & 0xfff00000) >> 20;
            regs[rd] = regs[rs1] + imm;
            return update_pc();
        }
        case 0x33: { // add
            regs[rd] = regs[rs1] + regs[rs2];
            return update_pc();
        }
        default: {
            throw std::invalid_argument("Invalid opcode: " + std::to_string(opcode));
        }
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