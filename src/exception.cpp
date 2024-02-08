//
// Created by Jie Wei on 2024/2/5.
//

#include "exception.h"

namespace crvemu {
    Exception::Exception(Type type, uint64_t value) : type(type), value(value) {}

    std::ostream& operator<<(std::ostream& os, const Exception& ex) {
        switch (ex.type) {
            case Exception::Type::InstructionAddrMisaligned:
                os << "Instruction address misaligned " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::InstructionAccessFault:
                os << "Instruction access fault " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::IllegalInstruction:
                os << "Illegal instruction " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::Breakpoint:
                os << "Breakpoint " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::LoadAccessMisaligned:
                os << "Load access " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::LoadAccessFault:
                os << "Load access fault " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::StoreAMOAddrMisaligned:
                os << "Store or AMO address misaligned " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::StoreAMOAccessFault:
                os << "Store or AMO access fault " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::EnvironmentCallFromUMode:
                os << "Environment call from U-mode " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::EnvironmentCallFromSMode:
                os << "Environment call from S-mode " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::EnvironmentCallFromMMode:
                os << "Environment call from M-mode " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::InstructionPageFault:
                os << "Instruction page fault " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::LoadPageFault:
                os << "Load page fault " << std::hex << "0x" << ex.value;
            break;
            case Exception::Type::StoreAMOPageFault:
                os << "Store or AMO page fault " << std::hex << "0x" << ex.value;
            break;
        }
        return os;
    }

    uint64_t Exception::getValue() const {
        return value;
    }

    uint64_t Exception::getCode() const {
        switch (type) {
            case Type::InstructionAddrMisaligned:
                return 0;
            case Type::InstructionAccessFault:
                return 1;
            case Type::IllegalInstruction:
                return 2;
            case Type::Breakpoint:
                return 3;
            case Type::LoadAccessMisaligned:
                return 4;
            case Type::LoadAccessFault:
                return 5;
            case Type::StoreAMOAddrMisaligned:
                return 6;
            case Type::StoreAMOAccessFault:
                return 7;
            case Type::EnvironmentCallFromUMode:
                return 8;
            case Type::EnvironmentCallFromSMode:
                return 9;
            case Type::EnvironmentCallFromMMode:
                return 11;
            case Type::InstructionPageFault:
                return 12;
            case Type::LoadPageFault:
                return 13;
            case Type::StoreAMOPageFault:
                return 15;
        }
    }

    bool Exception::isFatal() const {
        switch (type) {
            case Type::InstructionAddrMisaligned:
            case Type::InstructionAccessFault:
            case Type::LoadAccessFault:
            case Type::StoreAMOAddrMisaligned:
            case Type::StoreAMOAccessFault:
            case Type::IllegalInstruction:
                return true;
            default:
                return false;
        }
    }
}