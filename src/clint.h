// clint.h

#pragma once
#include <cstdint>
#include "exception.h"
#include "param.h"

namespace crvemu {

class Clint {
 private:
  uint64_t mtime;
  uint64_t mtimecmp;

 public:
  Clint();

  uint64_t load(uint64_t addr, uint64_t size);
  void store(uint64_t addr, uint64_t size, uint64_t value);
};

}