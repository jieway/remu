#include "instructions.h"

#include <bitset>
#include <functional>  // for std::hash
#include <iostream>
#include <optional>
#include <unordered_map>

#include "log.h"

namespace std {
    template <>
    struct hash<std::tuple<uint32_t, uint32_t>> {
        size_t operator()(const std::tuple<uint32_t, uint32_t>& t) const {
            return hash<uint32_t>()(std::get<0>(t)) ^ hash<uint32_t>()(std::get<1>(t));
        }
    };
}

namespace crvemu {

// 拆包解包
std::tuple<uint32_t, uint32_t, uint32_t> unpackInstruction(uint32_t inst) {
  return {
    (inst >> 7) & 0x1f,   // rd
    (inst >> 15) & 0x1f,  // rs1
    (inst >> 20) & 0x1f  // rs2
  };
}

std::optional<uint64_t> executeAddi(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  LOG(INFO, "ADDI: x", rd, " = x", rs1, " + ", immediate);
  cpu.regs[rd] = cpu.regs[rs1] + immediate;
  return cpu.update_pc();
}

std::optional<uint64_t> executeSlli(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);

  LOG(INFO, "SLLI: x", rd, " = x", rs1, " << ", (immediate & 0x3f));
  cpu.regs[rd] = cpu.regs[rs1] << (immediate & 0x3f);
  return cpu.update_pc();
}

std::optional<uint64_t> executeSlti(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);

  LOG(INFO, "SLTI: x", rd, " = (x", rs1, " < ", immediate, ") ? 1 : 0");
  cpu.regs[rd] = (cpu.regs[rs1] < static_cast<uint64_t>(immediate)) ? 1 : 0;
  return cpu.update_pc();
}

std::optional<uint64_t> executeSltiu(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);

  LOG(INFO, "SLTIU: x", rd, " = (x", rs1, " < ", immediate, ") ? 1 : 0");
  cpu.regs[rd] = (cpu.regs[rs1] < static_cast<unsigned int>(immediate)) ? 1 : 0;
  return cpu.update_pc();
}

std::optional<uint64_t> executeXori(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);

  LOG(INFO, "XORI: x", rd, " = x", rs1, " ^ ", immediate);
  cpu.regs[rd] = cpu.regs[rs1] ^ immediate;
  return cpu.update_pc();
}

std::optional<uint64_t> executeSrli(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);

  // "对于 RV64I，移位量被编码在 I-immediate 字段的低 6 位中。"
  uint32_t shamt = static_cast<uint32_t>(immediate & 0x3f);

  LOG(INFO, "SRLI: x", rd, " = x", rs1, " >> ", shamt);
  cpu.regs[rd] = cpu.regs[rs1] >> shamt;
  return cpu.update_pc();
}

std::optional<uint64_t> executeSrai(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);

  // 在 I 类型指令中，immediate 字段（I-immediate field）通常用来表示一个立即数，
  // 而对于右移类指令，如算术右移指令（srai），这个立即数的低6位通常用来表示右移的位数。
  uint32_t shamt = static_cast<uint32_t>(immediate & 0x3f);

  LOG(INFO, "SRAI: x", rd, " = x", rs1, " >> ", shamt, " (arithmetic right shift)");
  cpu.regs[rd] = static_cast<uint64_t>(static_cast<int64_t>(cpu.regs[rs1]) >> shamt);
  return cpu.update_pc();
}

std::optional<uint64_t> executefunct70X5(Cpu& cpu, uint32_t inst) {
  uint32_t funct7 = (inst & 0xfe000000) >> 25;
  LOG(INFO, "Executing srli or srai funct7: 0x" , std::hex , funct7 , std::dec);
  switch (funct7) {
    // srli
    case 0x00: {
      return executeSrli(cpu, inst);
    }
    // srai
    case 0x20: {
      return executeSrai(cpu, inst);
    }
    default:
      return std::nullopt;
  }
}

std::optional<uint64_t> executeOri(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  LOG(INFO, "ORI: x", rd , " = x" , rs1 , " | ", immediate);
  cpu.regs[rd] = cpu.regs[rs1] | immediate;
  return cpu.update_pc();
}

std::optional<uint64_t> executeAndi(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  LOG(INFO, "ANDI: x", rd , " = x" , rs1 , " & ", immediate);
  cpu.regs[rd] = cpu.regs[rs1] & immediate;
  return cpu.update_pc();
}

std::optional<uint64_t> executeAdd(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  LOG(INFO, "ADD: x" , rd , " = x" , rs1 , " + x" , rs2);
  cpu.regs[rd] = cpu.regs[rs1] + cpu.regs[rs2];
  return cpu.update_pc();
}

std::optional<uint64_t> InstructionExecutor::execute(Cpu& cpu, uint32_t inst) {
  uint32_t opcode = inst & 0x7f;
  uint32_t funct3 = (inst >> 12) & 0x7;

  // x0 is hardwired zero
  cpu.regs[0] = 0;
  LOG(INFO, "Executing instruction: 0x", std::hex, opcode, ", funct3: 0x", funct3, std::dec);

  std::unordered_map<
      std::tuple<uint32_t, uint32_t>,
      std::function<std::optional<uint64_t>(Cpu&, uint32_t)>
  > instructionMap = {
    {std::make_tuple(0x03, 0x0), executeLb},
    {std::make_tuple(0x13, 0x0), executeAddi},
    {std::make_tuple(0x13, 0x1), executeSlli},
    {std::make_tuple(0x13, 0x2), executeSlti},
    {std::make_tuple(0x13, 0x3), executeSltiu},
    {std::make_tuple(0x13, 0x4), executeXori},
    {std::make_tuple(0x13, 0x5), executefunct70X5},
    {std::make_tuple(0x13, 0x6), executeOri},
    {std::make_tuple(0x13, 0x7), executeAndi},
    {std::make_tuple(0x23, 0x0), executeStoreByte},
    {std::make_tuple(0x33, 0x0), executeAdd},
  };

  auto it = instructionMap.find({opcode, funct3});
  if (it != instructionMap.end()) {
    return it->second(cpu, inst);
  }
  return std::nullopt;  // 确保所有可能的执行路径都有明确的返回值
}

}