// clint.h

#pragma once
#include <cstdint>
#include "exception.h"
#include "param.h"

namespace cemu {

class Clint {
 public:
  Clint() : mtime(0), mtimecmp(0) {}

  uint64_t load(uint64_t addr, uint64_t size);
  void store(uint64_t addr, uint64_t size, uint64_t value);

 private:
  uint64_t mtime;     // Machine time
  uint64_t mtimecmp;  // Machine time compare
};

}