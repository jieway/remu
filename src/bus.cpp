// Bus.cpp
#include <string>
#include "bus.h"
#include "param.h"

namespace crvemu {
Bus::Bus(const std::vector<uint8_t>& code) : dram(code) {}

std::optional<uint64_t> Bus::load(uint64_t addr, uint64_t size) {
  if (addr >= DRAM_BASE && addr <= DRAM_END) {
    return dram.load(addr, size);
  }
  return std::nullopt;
}

bool Bus::store(uint64_t addr, uint64_t size, uint64_t value) {
  if (addr >= DRAM_BASE && addr <= DRAM_END) {
    return dram.store(addr, size, value);
  } else {
    return false;
  }
}
}