#include "exception.h"
#include "instructions.h"
#include <iostream>
#include <unordered_map>
#include <functional>  // for std::hash
#include <optional>

namespace std {
    template <>
    struct hash<std::pair<unsigned int, unsigned int>> {
        size_t operator()(const std::pair<unsigned int, unsigned int>& p) const {
            // Use a combination of hashes for the two elements
            return hash<unsigned int>()(p.first) ^ hash<unsigned int>()(p.second);
        }
    };
}

std::optional<uint64_t> executeAddi(Cpu& cpu, uint32_t rd, uint32_t rs1, int64_t immediate) {
    std::cout << "ADDI: x" << rd << " = x" << rs1 << " + " << immediate << std::endl;
    cpu.regs[rd] = cpu.regs[rs1] + immediate;
    return cpu.update_pc();
}


std::optional<uint64_t> executeSlli(Cpu& cpu, uint32_t rd, uint32_t rs1, int64_t immediate) {
    std::cout << "SLLI: x" << rd << " = x" << rs1 << " << " << (immediate & 0x3f) << std::endl;
    cpu.regs[rd] = cpu.regs[rs1] << (immediate & 0x3f);
    return cpu.update_pc();
}


std::optional<uint64_t> executeAdd(Cpu& cpu, uint32_t rd, uint32_t rs1, uint32_t rs2) {
    std::cout << "ADD: x" << rd << " = x" << rs1 << " + x" << rs2 << std::endl;
    cpu.regs[rd] = cpu.regs[rs1] + cpu.regs[rs2];
    return cpu.update_pc();
}

std::optional<uint64_t> InstructionExecutor::execute(Cpu& cpu,
    uint32_t opcode, uint32_t rd, uint32_t rs1, uint32_t rs2,
    uint32_t funct3, uint32_t funct7, int64_t immediate) {
    try {
        // x0 is hardwired zero
        cpu.regs[0] = 0;

        std::cout << "Executing instruction: 0x" << std::hex << opcode << std::dec << std::endl;

        // Define a map of lambdas for each opcode and funct3 combination
        std::unordered_map<
            std::pair<uint32_t, uint32_t>,
            std::function<std::optional<uint64_t>(Cpu&, uint32_t, uint32_t, int64_t)>
        > instructionMap = {
            {{0x13, 0x0}, executeAddi},
            {{0x13, 0x1}, executeSlli},
            {{0x33, 0x0}, executeAdd},
            // Add more entries for other instructions as needed
        };

        // Find the corresponding lambda function in the map
        auto it = instructionMap.find({opcode, funct3});
        if (it != instructionMap.end()) {
            return it->second(cpu, rd, rs1, immediate);
        } else {
            throw Exception(Exception::Type::IllegalInstruction, opcode);
        }
    } catch (const Exception& e) {
        std::cerr << "Exception execute : " << e << std::endl;
        return std::nullopt;  // Using std::optional to represent exceptions
    }
}