// Bus.h
#pragma once

#include <vector>
#include <cstdint>
#include "dram.h" // 包含Dram类的定义

namespace cemu {

class Bus {
public:
  Bus(const std::vector<uint8_t>& code);

  std::optional<uint64_t> load(uint64_t addr, uint64_t size);
  bool store(uint64_t addr, uint64_t size, uint64_t value);

private:
  Dram dram;
};

}
