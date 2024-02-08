// Dram.cpp
#include <stdexcept>
#include "dram.h"
#include "exception.h"
#include "param.h"

namespace crvemu {
    Dram::Dram(const std::vector<uint8_t>& code) {
        dram.resize(DRAM_SIZE, 0); // Initialize DRAM with 0s
        // Copy code into dram
        std::copy(code.begin(), code.end(), dram.begin());
    }

    std::optional<uint64_t> Dram::load(uint64_t addr, uint64_t size) {
      if (size != 8 && size != 16 && size != 32 && size != 64) {
        Exception ex(Exception::Type::LoadAccessFault, addr);
        ex.printException(ex.getType(), ex.getValue());
        return std::nullopt;
      }
      uint64_t nbytes = size / 8;
      std::size_t index = (addr - DRAM_BASE);
      if (index + nbytes > dram.size()) {
        Exception ex(Exception::Type::LoadAccessFault, addr);
        ex.printException(ex.getType(), ex.getValue());
        return std::nullopt;
      }

      uint64_t value = 0;
      for (uint64_t i = 0; i < nbytes; ++i) {
        value |= static_cast<uint64_t>(dram[index + i]) << (i * 8);
      }
      return value;
    }

    bool Dram::store(uint64_t addr, uint64_t size, uint64_t value) {
      if (size != 8 && size != 16 && size != 32 && size != 64) {
        return false;
      }
      uint64_t nbytes = size / 8;
      std::size_t index = (addr - DRAM_BASE);
      if (index + nbytes > dram.size()) {
        return false;
      }

      for (uint64_t i = 0; i < nbytes; ++i) {
        dram[index + i] = (value >> (i * 8)) & 0xFF;
      }
      return true;
    }
}
