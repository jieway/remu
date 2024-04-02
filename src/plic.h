//
// Created by Jie Wei on 2024/4/2.
//

#pragma once
#include <cstdint>
#include "exception.h"

namespace crvemu {

// 这个类的设计是为了模拟PLIC的功能，使得在模拟器中可以像在真实硬件中一样处理中断。
// 这个类的方法提供了对PLIC寄存器的读写操作，这些操作在处理中断时是必需的。
class Plic {
 public:
  Plic() : pending(0), senable(0), spriority(0), sclaim(0) {}

  // 读取和写入PLIC的寄存器
  uint64_t load(uint64_t addr, uint64_t size);
  void store(uint64_t addr, uint64_t size, uint64_t value);

 private:
  uint64_t pending;     // 对应PLIC的挂起寄存器
  uint64_t senable;     // 对应PLIC的使能寄存器
  uint64_t spriority;   // 对应PLIC的优先级寄存器
  uint64_t sclaim;      // 对应PLIC的索引寄存器
};

}

