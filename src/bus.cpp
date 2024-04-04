// Bus.cpp
#include <string>
#include <iostream>
#include "bus.h"
#include "param.h"
#include "log.h"
#include "exception.h"

namespace cemu {

Bus::Bus(const std::vector<uint8_t>& code) : dram(code) {}

std::optional<uint64_t> Bus::load(uint64_t addr, uint64_t size) {
  if (addr >= DRAM_BASE && addr <= DRAM_END) {
    LOG(INFO, "Bus loading from DRAM address ", std::hex, addr, " with size ", size, " bytes.");
    return dram.load(addr, size);
  }
  throw Exception(ExceptionType::LoadAccessFault, addr);
}

bool Bus::store(uint64_t addr, uint64_t size, uint64_t value) {
  if (addr >= DRAM_BASE && addr <= DRAM_END) {
    LOG(INFO, "Bus storing value ", std::hex, value, " at DRAM address ", addr, " with size ", size, " bytes.");
    return dram.store(addr, size, value);
  }
  throw Exception(ExceptionType::StoreAMOAccessFault, addr);
}


}