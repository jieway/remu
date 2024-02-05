//
// Created by Jie Wei on 2024/2/5.
//
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <functional>
#include <optional>
#include <iostream>
#include "instructions.h"
#include "cup.h"

class InstructionExecutor {
public:
    static std::optional<uint64_t> execute(Cpu& cpu, uint32_t opcode, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t funct3, uint32_t funct7, int64_t immediate);
};

#endif
