//
// Created by Jie Wei on 2024/4/4.
//

#include "interrupt.h"

namespace cemu {

Interrupt::Interrupt(InterruptType type) : type_(type) {}

uint64_t Interrupt::code() const {
  switch (type_) {
    case InterruptType::SupervisorSoftwareInterrupt:
      return 1 | MASK_INTERRUPT_BIT;
    case InterruptType::MachineSoftwareInterrupt:
      return 3 | MASK_INTERRUPT_BIT;
    case InterruptType::SupervisorTimerInterrupt:
      return 5 | MASK_INTERRUPT_BIT;
    case InterruptType::MachineTimerInterrupt:
      return 7 | MASK_INTERRUPT_BIT;
    case InterruptType::SupervisorExternalInterrupt:
      return 9 | MASK_INTERRUPT_BIT;
    case InterruptType::MachineExternalInterrupt:
      return 11 | MASK_INTERRUPT_BIT;
    default:
      return 0;
  }
}

}


