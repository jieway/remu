//
// Created by Jie Wei on 2024/2/5.
//
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include "cup.h"

namespace cemu {

class InstructionExecutor {
public:
  static std::optional<uint64_t> execute(Cpu& cpu, uint32_t inst);
};

}
