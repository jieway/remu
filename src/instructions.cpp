#include "exception.h"
#include "instructions.h"
#include <iostream>
#include <unordered_map>
#include <functional>  // for std::hash
#include <optional>

namespace std {
    template <>
    struct hash<std::tuple<uint32_t, uint32_t>> {
        size_t operator()(const std::tuple<uint32_t, uint32_t>& t) const {
            // Use a combination of hashes for the three elements
            return hash<uint32_t>()(std::get<0>(t)) ^ hash<uint32_t>()(std::get<1>(t));
        }
    };
}

namespace cemu {

    std::optional<uint64_t> executeAddi(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        int64_t immediate = static_cast<int64_t>(inst & 0xfff00000) >> 20;

        std::cout << "ADDI: x" << rd << " = x" << rs1 << " + " << immediate << std::endl;
        cpu.regs[rd] = cpu.regs[rs1] + immediate;
        return cpu.update_pc();
    }

    std::optional<uint64_t> executeSlli(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        int64_t immediate = static_cast<int64_t>(inst & 0xfff00000) >> 20;

        std::cout << "SLLI: x" << rd << " = x" << rs1 << " << " << (immediate & 0x3f) << std::endl;
        cpu.regs[rd] = cpu.regs[rs1] << (immediate & 0x3f);
        return cpu.update_pc();
    }

    std::optional<uint64_t> executeSlti(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        int64_t imm = static_cast<int64_t>(inst & 0xfff00000) >> 20;
        std::cout << "SLTI: x" << rd << " = (x" << rs1 << " < " << imm << ") ? 1 : 0" << std::endl;
        cpu.regs[rd] = (cpu.regs[rs1] < static_cast<uint64_t>(imm)) ? 1 : 0;
        return cpu.update_pc();
    }

    std::optional<uint64_t> executeSltiu(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        int64_t imm = static_cast<int64_t>(inst & 0xfff00000) >> 20;
        std::cout << "SLTIU: x" << rd << " = (x" << rs1 << " < " << imm << ") ? 1 : 0" << std::endl;
        cpu.regs[rd] = (cpu.regs[rs1] < static_cast<unsigned int>(imm)) ? 1 : 0;
        return cpu.update_pc();
    }

    std::optional<uint64_t> executeXori(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        uint32_t rs2 = (inst >> 20) & 0x1f;
        int64_t imm = static_cast<int64_t>(inst & 0xfff00000) >> 20;
        std::cout << "XORI: x" << rd << " = x" << rs1 << " ^ " << imm << std::endl;
        cpu.regs[rd] = cpu.regs[rs1] ^ imm;
        return cpu.update_pc();
    }
    std::optional<uint64_t> executeSrli(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        // imm[11:0] = inst[31:20]
        uint64_t imm = static_cast<uint64_t>((static_cast<int32_t>(inst & 0xfff00000) >> 20) & 0xfff);

        // "The shift amount is encoded in the lower 6 bits of the I-immediate field for RV64I."
        uint32_t shamt = static_cast<uint32_t>(imm & 0x3f);

        std::cout << "SRLI: x" << rd << " = x" << rs1 << " >> " << shamt << std::endl;
        cpu.regs[rd] = cpu.regs[rs1] >> shamt;
        return cpu.update_pc();
    }

    std::optional<uint64_t> executeSrai(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        // imm[11:0] = inst[31:20]
        uint64_t imm = static_cast<uint64_t>((static_cast<int32_t>(inst & 0xfff00000) >> 20) & 0xfff);

        // "The shift amount is encoded in the lower 6 bits of the I-immediate field for RV64I."
        uint32_t shamt = static_cast<uint32_t>(imm & 0x3f);

        std::cout << "SRAI: x" << rd << " = x" << rs1 << " >> " << shamt << " (arithmetic right shift)" << std::endl;
        int64_t signed_rs1 = static_cast<int64_t>(cpu.regs[rs1]);
        cpu.regs[rd] = static_cast<uint64_t>(signed_rs1 >> shamt);
        return cpu.update_pc();
    }

    std::optional<uint64_t> executefunct70X5(Cpu& cpu, uint32_t inst) {
        uint32_t funct7 = (inst & 0xfe000000) >> 25;
        switch (funct7 >> 1) {
            // srli
            case 0x00: {
                return executeSrli(cpu, inst);
            }
            // srai
            case 0x10: {
                return executeSrli(cpu, inst);
            }
            default:
                throw Exception(Exception::Type::IllegalInstruction, inst);
        }
    }

    std::optional<uint64_t> executeOri(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        // imm[11:0] = inst[31:20]
        uint64_t imm = static_cast<uint64_t>((static_cast<int32_t>(inst & 0xfff00000) >> 20) & 0xfff);
        std::cout << "ORI: x" << rd << " = x" << rs1 << " | " << imm << std::endl;
        cpu.regs[rd] = cpu.regs[rs1] | imm;
        return cpu.update_pc();
    }

    std::optional<uint64_t> executeAndi(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;

        // imm[11:0] = inst[31:20]
        uint64_t imm = static_cast<uint64_t>((static_cast<int32_t>(inst & 0xfff00000) >> 20) & 0xfff);
        std::cout << "ANDI: x" << rd << " = x" << rs1 << " & " << imm << std::endl;
        cpu.regs[rd] = cpu.regs[rs1] & imm;
        return cpu.update_pc();
    }

    std::optional<uint64_t> executeAdd(Cpu& cpu, uint32_t inst) {
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        uint32_t rs2 = (inst >> 20) & 0x1f;
        std::cout << "ADD: x" << rd << " = x" << rs1 << " + x" << rs2 << std::endl;
        cpu.regs[rd] = cpu.regs[rs1] + cpu.regs[rs2];
        return cpu.update_pc();
    }

    std::optional<uint64_t> InstructionExecutor::execute(Cpu& cpu, uint32_t inst) {
        try {
            uint32_t opcode = inst & 0x7f;
            uint32_t funct3 = (inst >> 12) & 0x7;

            // x0 is hardwired zero
            cpu.regs[0] = 0;
            std::cout << "Executing instruction: 0x" << std::hex << opcode << std::dec << std::endl;

            std::unordered_map<
                std::tuple<uint32_t, uint32_t>,
                std::function<std::optional<uint64_t>(Cpu&, uint32_t)>
            > instructionMap = {
                {std::make_tuple(0x13, 0x0), executeAddi},
                {std::make_tuple(0x13, 0x1), executeSlli},
                {std::make_tuple(0x13, 0x2), executeSlti},
                {std::make_tuple(0x13, 0x3), executeSltiu},
                {std::make_tuple(0x13, 0x4), executeXori},
                {std::make_tuple(0x13, 0x5), executefunct70X5},
                {std::make_tuple(0x13, 0x6), executeOri},
                {std::make_tuple(0x13, 0x7), executeAndi},
                {std::make_tuple(0x33, 0x0), executeAdd},
            };

            auto it = instructionMap.find({opcode, funct3});
            if (it != instructionMap.end()) {
                return it->second(cpu, inst);
            }

        } catch (const Exception& e) {
            std::cerr << "Exception InstructionExecutor::execute: " << e << std::endl;
            return std::nullopt;
        }
    }
}