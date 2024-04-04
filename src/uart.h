// uart.h

#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "bus.h"
#include "exception.h"
#include "param.h"

namespace cemu {

class Uart {
 public:
  Uart();
  bool is_interrupting();
  uint64_t load(uint64_t addr, uint64_t size);
  void store(uint64_t addr, uint64_t size, uint64_t value);

  std::vector<uint8_t> uart;
  std::condition_variable cv;
  std::atomic<bool> interrupt;
  std::mutex mtx;
};

}