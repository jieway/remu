
#pragma once
#include <cstdint>

namespace cemu {

#include <cstdint>

enum class InterruptType {
  SupervisorSoftwareInterrupt,    // 软中断
  MachineSoftwareInterrupt,       // 机器软中断
  SupervisorTimerInterrupt,       // 监管态计时中断
  MachineTimerInterrupt,          // 机器态计时中断
  SupervisorExternalInterrupt,    // 监管态外部中断
  MachineExternalInterrupt,       // 机器态外部中断
};

class Interrupt {
 public:
  static const uint64_t MASK_INTERRUPT_BIT = 1ull << 63;  // 中断位
  Interrupt(InterruptType type);          // 构造函数
  uint64_t code() const;                  // 获取中断码

 private:
  InterruptType type_;                       // 中断类型
};

}


