// clint.cpp

#include "clint.h"
#include <stdexcept>

namespace cemu {

uint64_t Clint::load(uint64_t addr, uint64_t size) {
  if (size != 64) {
    throw Exception(ExceptionType::LoadAccessFault, addr);
  }
  switch (addr) {
    case CLINT_MTIMECMP:
      return mtimecmp;
    case CLINT_MTIME:
      return mtime;
    default:
      throw Exception(ExceptionType::LoadAccessFault, addr);
  }
}

void Clint::store(uint64_t addr, uint64_t size, uint64_t value) {
  if (size != 64) {
    throw Exception(ExceptionType::StoreAMOAccessFault, addr);
  }
  switch (addr) {
    case CLINT_MTIMECMP:
      mtimecmp = value;
      break;
    case CLINT_MTIME:
      mtime = value;
      break;
    default:
      throw Exception(ExceptionType::StoreAMOAccessFault, addr);
  }
}

}