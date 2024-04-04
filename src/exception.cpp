//
// Created by Jie Wei on 2024/4/2.
//

#include "exception.h"

namespace cemu {

Exception::Exception(ExceptionType type, uint64_t value)
    : type(type), value(value) {}

uint64_t Exception::getValue() const {
  return value;
}

ExceptionType Exception::getType() const {
  return type;
}

bool Exception::isFatal() const {
  switch (type) {
    case ExceptionType::InstructionAddrMisaligned:
    case ExceptionType::InstructionAccessFault:
    case ExceptionType::LoadAccessFault:
    case ExceptionType::StoreAMOAddrMisaligned:
    case ExceptionType::StoreAMOAccessFault:
    case ExceptionType::IllegalInstruction:
      return true;
    default:
      return false;
  }
}

std::string Exception::what() const {
  std::ostringstream oss;
  oss << "Exception type: " << static_cast<int>(type)
      << ", value: " << value;
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Exception& e) {
  os << std::hex;
  switch (e.type) {
    case ExceptionType::InstructionAddrMisaligned:
      os << "Instruction address misaligned " << e.value;
      break;
    case ExceptionType::InstructionAccessFault:
      os << "Instruction access fault " << e.value;
      break;
    case ExceptionType::IllegalInstruction:
      os << "Illegal instruction " << e.value;
      break;
    case ExceptionType::Breakpoint:
      os << "Breakpoint " << e.value;
      break;
    case ExceptionType::LoadAccessMisaligned:
      os << "Load access " << e.value;
      break;
    case ExceptionType::LoadAccessFault:
      os << "Load access fault " << e.value;
      break;
    case ExceptionType::StoreAMOAddrMisaligned:
      os << "Store or AMO address misaliged " << e.value;
      break;
    case ExceptionType::StoreAMOAccessFault:
      os << "Store or AMO access fault " << e.value;
      break;
    case ExceptionType::EnvironmentCallFromUMode:
      os << "Environment call from U-mode " << e.value;
      break;
    case ExceptionType::EnvironmentCallFromSMode:
      os << "Environment call from S-mode " << e.value;
      break;
    case ExceptionType::EnvironmentCallFromMMode:
      os << "Environment call from M-mode " << e.value;
      break;
    case ExceptionType::InstructionPageFault:
      os << "Instruction page fault " << e.value;
      break;
    case ExceptionType::LoadPageFault:
      os << "Load page fault " << e.value;
      break;
    case ExceptionType::StoreAMOPageFault:
      os << "Store or AMO page fault " << e.value;
      break;
  }
  return os;
}

}