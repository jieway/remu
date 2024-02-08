//
// Created by Jie Wei on 2024/2/5.
//
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <functional>
#include <optional>
#include "instructions.h"
#include "cup.h"

namespace crvemu {
    class InstructionExecutor {
    public:
        static std::optional<uint64_t> execute(Cpu& cpu, uint32_t inst);
    };
}

#endif
