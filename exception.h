//
// Created by Jie Wei on 2024/2/5.
//

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <cstdint>
#include <iostream>

class Exception {
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

    bool isFatal() const;
};

#endif //EXCEPTION_H
