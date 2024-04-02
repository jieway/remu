// Dram.cpp
#include <iostream>
#include "dram.h"
#include "param.h"
#include "log.h"
#include "exception.h"

namespace cemu {

Dram::Dram(const std::vector<uint8_t>& code) {
  dram.resize(DRAM_SIZE, 0); // Initialize DRAM with 0s
  // Copy code into dram
  std::copy(code.begin(), code.end(), dram.begin());
}

std::optional<uint64_t> Dram::load(uint64_t addr, uint64_t size) {
  if (size != 8 && size != 16 && size != 32 && size != 64) {
    throw Exception(ExceptionType::LoadAccessFault, addr);
  }
  uint64_t nbytes = size / 8;
  std::size_t index = (addr - DRAM_BASE);
  if (index + nbytes > dram.size()) {
    throw Exception(ExceptionType::LoadAccessFault, addr);
  }

  uint64_t value = 0;
  for (uint64_t i = 0; i < nbytes; ++i) {
    value |= static_cast<uint64_t>(dram[index + i]) << (i * 8);
  }

  LOG(INFO, "DRAM load successful. Value: ", value);
  return value;
}

bool Dram::store(uint64_t addr, uint64_t size, uint64_t value) {
  if (size != 8 && size != 16 && size != 32 && size != 64) {
    throw Exception(ExceptionType::StoreAMOAccessFault, addr);
  }

  uint64_t nbytes = size / 8;
  std::size_t index = (addr - DRAM_BASE);
  if (index + nbytes > dram.size()) {
    throw Exception(ExceptionType::StoreAMOAccessFault, addr);
  }

  for (uint64_t i = 0; i < nbytes; ++i) {
    dram[index + i] = (value >> (i * 8)) & 0xFF;
  }

  LOG(INFO, "DRAM store successful. Value: ", value, " at address ", std::hex, addr, " with size ", size, " bytes.");
  return true;
}

}
