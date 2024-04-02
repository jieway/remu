//
// Created by Jie Wei on 2024/4/2.
//

#include <cstdint>
#include "exception.h"
#include "plic.h"
#include "param.h"

namespace crvemu {

uint64_t Plic::load(uint64_t addr, uint64_t size) {
  if (size != 32) {
    throw Exception(ExceptionType::LoadAccessFault, addr);
  }
  switch (addr) {
    case PLIC_PENDING:
      return pending;
    case PLIC_SENABLE:
      return senable;
    case PLIC_SPRIORITY:
      return spriority;
    case PLIC_SCLAIM:
      return sclaim;
    default:
      return 0;
  }
}


void Plic::store(uint64_t addr, uint64_t size, uint64_t value) {
  if (size != 32) {
    throw Exception(ExceptionType::StoreAMOAccessFault, addr);
  }
  switch (addr) {
    case PLIC_PENDING:
      pending = value;
      break;
    case PLIC_SENABLE:
      senable = value;
      break;
    case PLIC_SPRIORITY:
      spriority = value;
      break;
    case PLIC_SCLAIM:
      sclaim = value;
      break;
    default:
      break;
  }
}

}

