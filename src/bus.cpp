// Bus.cpp
#include <string>
#include <iostream>
#include "bus.h"
#include "param.h"
#include "log.h"

namespace crvemu {
Bus::Bus(const std::vector<uint8_t>& code) : dram(code) {}

std::optional<uint64_t> Bus::load(uint64_t addr, uint64_t size) {
  if (addr >= DRAM_BASE && addr <= DRAM_END) {
    LOG(INFO, "Bus loading from DRAM address ", addr, " with size ", size, " bytes.");
    return dram.load(addr, size);
  }
  LOG(ERROR, "Invalid address for load operation: ", addr);
  return std::nullopt;
}

bool Bus::store(uint64_t addr, uint64_t size, uint64_t value) {
  if (addr >= DRAM_BASE && addr <= DRAM_END) {
    LOG(INFO, "Bus storing value ", value, " at DRAM address ", addr, " with size ", size, " bytes.");
    return dram.store(addr, size, value);
  } else {
    LOG(ERROR, "Invalid address for store operation: ", addr);
    return false;
  }
}

}