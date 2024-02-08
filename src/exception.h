//
// Created by Jie Wei on 2024/2/5.
//

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <cstdint>
#include <iostream>
#include <iomanip>

namespace crvemu {
    class Exception  {
    public:
        enum class Type {
            InstructionAddrMisaligned,
            InstructionAccessFault,
            IllegalInstruction,
            Breakpoint,
            LoadAccessMisaligned,
            LoadAccessFault,
            StoreAMOAddrMisaligned,
            StoreAMOAccessFault,
            EnvironmentCallFromUMode,
            EnvironmentCallFromSMode,
            EnvironmentCallFromMMode,
            InstructionPageFault,
            LoadPageFault,
            StoreAMOPageFault
        };

    private:
        Type type;
        uint64_t value;

    public:
      Exception(Type type, uint64_t value);

      friend std::ostream& operator<<(std::ostream& os, const Exception& ex);

      uint64_t getValue() const;

      uint64_t getCode() const;

      Type Exception::getType() const {
        return type;
      }

      bool isFatal() const;

      void printException(Exception::Type type, uint64_t addr) {
        Exception ex(type, addr);
        std::cout << ex << std::endl;
        std::cout << "Value: 0x" << std::hex << std::setw(16) << std::setfill('0') << ex.getValue() << std::endl;
        std::cout << "Code: " << ex.getCode() << std::endl;
        std::cout << "Is Fatal: " << std::boolalpha << ex.isFatal() << std::endl;
      }
    };
}
#endif //EXCEPTION_H
