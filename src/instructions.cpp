
#include <bitset>
#include <functional>  // for std::hash
#include <iostream>
#include <optional>
#include <unordered_map>
#include "log.h"
#include "instructions.h"

namespace std {
template <>
struct hash<std::tuple<uint32_t, uint32_t>> {
  size_t operator()(const std::tuple<uint32_t, uint32_t>& t) const {
    return hash<uint32_t>()(std::get<0>(t)) ^ hash<uint32_t>()(std::get<1>(t));
  }
};

template <>
struct hash<std::tuple<uint32_t, uint32_t, uint32_t>> {
  size_t operator()(const std::tuple<unsigned int, unsigned int, unsigned int>& key) const {
    // 自定义哈希函数的实现
    size_t hash_value = std::hash<unsigned int>{}(std::get<0>(key));
    hash_value ^= std::hash<unsigned int>{}(std::get<1>(key)) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    hash_value ^= std::hash<unsigned int>{}(std::get<2>(key)) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};
}


namespace cemu {


// 拆包解包
std::tuple<uint32_t, uint32_t, uint32_t> unpackInstruction(uint32_t inst) {
  return {
    (inst >> 7) & 0x1f,   // rd
    (inst >> 15) & 0x1f,  // rs1
    (inst >> 20) & 0x1f  // rs2
  };
}

std::optional<uint64_t> executeFence(Cpu& cpu, uint32_t inst) {
  // 模拟在单个线程上按顺序执行指令，所以 fence 指令不执行任何操作。
  return cpu.update_pc();
}

std::optional<uint64_t> executeLb(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  uint64_t addr = cpu.regs[rs1] + immediate;

  LOG(INFO, "LB: x", rd, " = MEM[x", rs1, " + ", immediate, "]");
  auto val = cpu.load(addr, 8);
  if (val.has_value()) {
    cpu.regs[rd] = static_cast<uint64_t>(static_cast<int8_t>(val.value() & 0xff));  // Sign extend
    return cpu.update_pc();
  }

  LOG(ERROR, "LB FAIL!");
  return std::nullopt;
}

std::optional<uint64_t> executeLh(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  uint64_t addr = cpu.regs[rs1] + immediate;

  LOG(INFO, "LH: x", rd, " = MEM[x", rs1, " + ", immediate, "]");
  auto val = cpu.load(addr, 16);
  if (val.has_value()) {
    cpu.regs[rd] = static_cast<uint64_t>(static_cast<int16_t>(val.value() & 0xffff));  // Sign extend
    return cpu.update_pc();
  }

  LOG(ERROR, "LH FAIL!");
  return std::nullopt;
}

std::optional<uint64_t> executeLw(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  uint64_t addr = cpu.regs[rs1] + immediate;

  LOG(INFO, "LW: x", rd, " = MEM[x", rs1, " + ", immediate, "]");
  auto val = cpu.load(addr, 32);
  if (val.has_value()) {
    cpu.regs[rd] = static_cast<uint64_t>(static_cast<int32_t>(val.value() & 0xffffffff));  // Sign extend
    return cpu.update_pc();
  }

  LOG(ERROR, "LW FAIL!");
  return std::nullopt;
}

std::optional<uint64_t> executeLd(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  uint64_t addr = cpu.regs[rs1] + immediate;

  LOG(INFO, "LD: x", rd, " = MEM[x", rs1, " + ", immediate, "]");
  auto val = cpu.load(addr, 64);
  if (val.has_value()) {
    cpu.regs[rd] = val.value();
    return cpu.update_pc();
  }

  LOG(ERROR, "LD FAIL!");
  return std::nullopt;
}

std::optional<uint64_t> executeLbu(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  uint64_t addr = cpu.regs[rs1] + immediate;

  LOG(INFO, "LBU: x", rd, " = MEM[x", rs1, " + ", immediate, "]");
  auto val = cpu.load(addr, 8);
  if (val.has_value()) {
    cpu.regs[rd] = val.value() & 0xff;  // Zero extend
    return cpu.update_pc();
  }

  LOG(ERROR, "LBU FAIL!");
  return std::nullopt;
}


std::optional<uint64_t> executeLhu(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  uint64_t addr = cpu.regs[rs1] + immediate;

  LOG(INFO, "LHU: x", rd, " = MEM[x", rs1, " + ", immediate, "]");
  auto val = cpu.load(addr, 16);
  if (val.has_value()) {
    cpu.regs[rd] = val.value() & 0xffff;  // Zero extend
    return cpu.update_pc();
  }

  LOG(ERROR, "LHU FAIL!");
  return std::nullopt;
}


std::optional<uint64_t> executeLwu(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);
  uint64_t addr = cpu.regs[rs1] + immediate;

  LOG(INFO, "LWU: x", rd, " = MEM[x", rs1, " + ", immediate, "]");
  auto val = cpu.load(addr, 32);
  if (val.has_value()) {
    cpu.regs[rd] = val.value() & 0xffffffff;  // Zero extend
    return cpu.update_pc();
  }

  LOG(ERROR, "LWU FAIL!");
  return std::nullopt;
}





std::optional<uint64_t> executeSb(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  int64_t imm = ((static_cast<int32_t>(inst & 0xfe000000) >> 20) & 0xffffffffffffffe0) |
                ((inst >> 7) & 0x1f);
  uint64_t addr = cpu.regs[rs1] + imm;
  LOG(INFO, "SB: x", rd, " = x", rs1, " + ", imm, " addr: ", addr);
  bool isSuc =  cpu.store(addr, 8, cpu.regs[rs2]);

  if (isSuc) {
    LOG(INFO, "SB SUCCESS!");
    return cpu.update_pc();
  }

  LOG(ERROR, "SB FAIL!");
  return std::nullopt;
}

std::optional<uint64_t> executeStoreByte(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  int64_t imm = ((static_cast<int32_t>(inst & 0xfe000000) >> 20) & 0xffffffffffffffe0) |
                ((inst >> 7) & 0x1f);
  uint64_t addr = cpu.regs[rs1] + imm;
  LOG(INFO, "SB: x", rd, " = x", rs1, " + ", imm, " addr: ", addr);
  bool isSuc =  cpu.store(addr, 8, cpu.regs[rs2]);

  if (isSuc) {
    LOG(INFO, "SB SUCCESS!");
    return cpu.update_pc();
  }

  LOG(ERROR, "SB FAIL!");
  return std::nullopt;
}

std::optional<uint64_t> executeStoreHalf(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  int64_t imm = ((static_cast<int32_t>(inst & 0xfe000000) >> 20) & 0xffffffffffffffe0) |
                ((inst >> 7) & 0x1f);
  uint64_t addr = cpu.regs[rs1] + imm;
  cpu.store(addr, 16, cpu.regs[rs2]);
  return cpu.update_pc();
}

std::optional<uint64_t> executeStoreWord(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  int64_t imm = ((static_cast<int32_t>(inst & 0xfe000000) >> 20) & 0xffffffffffffffe0) |
                ((inst >> 7) & 0x1f);
  uint64_t addr = cpu.regs[rs1] + imm;
  cpu.store(addr, 32, cpu.regs[rs2]);
  return cpu.update_pc();
}

std::optional<uint64_t> executeStoreDouble(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  int64_t imm = ((static_cast<int32_t>(inst & 0xfe000000) >> 20) & 0xffffffffffffffe0) |
                ((inst >> 7) & 0x1f);
  uint64_t addr = cpu.regs[rs1] + imm;
  cpu.store(addr, 64, cpu.regs[rs2]);
  return cpu.update_pc();
}

std::optional<uint64_t> executeStore(Cpu& cpu, uint32_t inst) {
  auto [rs1, rs2, funct3] = unpackInstruction(inst);

  // imm[11:5|4:0] = inst[31:25|11:7]
  int64_t imm = ((static_cast<int32_t>(inst & 0xfe000000) >> 20) & 0xffffffffffffffe0) |
                ((inst >> 7) & 0x1f);

  uint64_t addr = cpu.regs[rs1] + imm;

  switch (funct3) {
    case 0x0: {
      // sb
      cpu.store(addr, 8, cpu.regs[rs2]);
      break;
    }
    case 0x1: {
      // sh
      cpu.store(addr, 16, cpu.regs[rs2]);
      break;
    }
    case 0x2: {
      // sw
      cpu.store(addr, 32, cpu.regs[rs2]);
      break;
    }
    case 0x3: {
      // sd
      cpu.store(addr, 64, cpu.regs[rs2]);
      break;
    }
    default:
      // Unreachable, as the funct3 values are known
        break;
  }

  return cpu.update_pc();
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

std::optional<uint64_t> executeSll(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  LOG(INFO, "SLL: x", rd, " = x", rs1, " << x", rs2);
  cpu.regs[rd] = cpu.regs[rs1] << cpu.regs[rs2];
  return cpu.update_pc();
}

std::optional<uint64_t> executeSlt(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  LOG(INFO, "SLT: x", rd, " = (x", rs1, " < x", rs2, ") ? 1 : 0");
  LOG(INFO, "Values: x", rs1, " = ", cpu.regs[rs1], ", x", rs2, " = ", cpu.regs[rs2]);
  cpu.regs[rd] = (static_cast<int64_t>(cpu.regs[rs1]) < static_cast<int64_t>(cpu.regs[rs2])) ? 1 : 0;
  return cpu.update_pc();
}

std::optional<uint64_t> executeXor(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  LOG(INFO, "XOR: x", rd, " = x", rs1, " ^ x", rs2);
  cpu.regs[rd] = cpu.regs[rs1] ^ cpu.regs[rs2];
  return cpu.update_pc();
}

std::optional<uint64_t> executeSrl(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  LOG(INFO, "SRL: x", rd, " = x", rs1, " >> x", rs2);
  cpu.regs[rd] = cpu.regs[rs1] >> cpu.regs[rs2];
  return cpu.update_pc();
}

std::optional<uint64_t> executeSra(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  LOG(INFO, "SRA: x", rd, " = x", rs1, " >> x", rs2, " (arithmetic right shift)");
  cpu.regs[rd] = static_cast<uint64_t>(static_cast<int64_t>(cpu.regs[rs1]) >> cpu.regs[rs2]);
  return cpu.update_pc();
}

std::optional<uint64_t> executeOr(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  LOG(INFO, "OR: x", rd, " = x", rs1, " | x", rs2);
  cpu.regs[rd] = cpu.regs[rs1] | cpu.regs[rs2];
  return cpu.update_pc();
}

std::optional<uint64_t> executeAnd(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  LOG(INFO, "AND: x", rd, " = x", rs1, " & x", rs2);
  cpu.regs[rd] = cpu.regs[rs1] & cpu.regs[rs2];
  return cpu.update_pc();
}

std::optional<uint64_t> executeAddw(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  LOG(INFO, "ADDW: x", rd, " = x", rs1, " + x", rs2);
  int64_t result = static_cast<int32_t>(cpu.regs[rs1]) + static_cast<int32_t>(cpu.regs[rs2]);
  cpu.regs[rd] = static_cast<uint64_t>(result);
  return cpu.update_pc();
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

std::optional<uint64_t> executeLui(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto immediate = static_cast<uint64_t>(inst & 0xfffff000);  // Extract the upper 20 bits
  LOG(INFO, "LUI: x", rd , " = ", immediate);
  cpu.regs[rd] = immediate;
  return cpu.update_pc();
}

std::optional<uint64_t> executeAUIPC(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto imm = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfffff000));

  LOG(INFO, "AUIPC: x", rd, " = pc + ", imm);
  cpu.regs[rd] = cpu.pc + imm;
  return cpu.update_pc();
}

std::optional<uint64_t> executeJAL(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto imm = ((inst & 0x80000000) ? 0xfff00000 : 0) |
             ((inst >> 20) & 0x7fe) |
             ((inst >> 9) & 0x800) |
             ((inst >> 12) & 0xff);

  LOG(INFO, "JAL: x", rd, " = pc + 4; pc = pc + ", imm);
  cpu.regs[rd] = cpu.pc + 4;
  return cpu.pc + imm;
}

std::optional<uint64_t> executeJALR(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto imm = static_cast<int64_t>(static_cast<int32_t>(inst & 0xfff00000) >> 20);

  uint64_t t = cpu.pc + 4;
  uint64_t new_pc = (cpu.regs[rs1] + imm) & ~1;

  LOG(INFO, "JALR: x", rd, " = pc + 4; pc = (x", rs1, " + ", imm, ") & ~1");
  cpu.regs[rd] = t;
  return new_pc;
}

std::optional<uint64_t> executeBEQ(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  int64_t imm = static_cast<int64_t>((((inst & 0x80000000) ? 0xFFF00000 : 0) |
                                   ((inst & 0x80) << 4) |
                                   ((inst >> 20) & 0x7E0) |
                                   ((inst >> 7) & 0x1E)) << 1) >> 1;  // Sign-extend

  if (cpu.regs[rs1] == cpu.regs[rs2]) {
    LOG(INFO, "BEQ: pc = pc + ", imm);
    return cpu.pc + imm;
  }
  return std::nullopt;
}

std::optional<uint64_t> executeCSR_RW(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto csr_addr = (inst & 0xfff00000) >> 20;

  // Load the value from the CSR register
  uint64_t t = cpu.csr.load(csr_addr);

  // Store the value from the rs1 register into the CSR register
  cpu.csr.store(csr_addr, cpu.regs[rs1]);

  // Store the original CSR value into the rd register
  cpu.regs[rd] = t;

  // Update the program counter
  return cpu.update_pc();
}

std::optional<uint64_t> executeCSR_RS(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto csr_addr = (inst & 0xfff00000) >> 20;

  // Load the value from the CSR register
  uint64_t t = cpu.csr.load(csr_addr);

  // Store the original CSR value into the rd register
  cpu.regs[rd] = t;

  // Perform bitwise OR operation between the CSR register value and the rs1 register value
  // and store the result back into the CSR register
  cpu.csr.store(csr_addr, t | cpu.regs[rs1]);

  // Update the program counter
  return cpu.update_pc();
}

std::optional<uint64_t> executeCSR_RC(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto csr_addr = (inst & 0xfff00000) >> 20;

  // Load the value from the CSR register
  uint64_t t = cpu.csr.load(csr_addr);

  // Store the original CSR value into the rd register
  cpu.regs[rd] = t;

  // Perform bitwise AND operation between the CSR register value and the bitwise NOT of the rs1 register value
  // and store the result back into the CSR register
  cpu.csr.store(csr_addr, t & ~cpu.regs[rs1]);

  // Update the program counter
  return cpu.update_pc();
}

std::optional<uint64_t> executeCSR_RWI(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto csr_addr = (inst & 0xfff00000) >> 20;

  // Load the value from the CSR register
  uint64_t t = cpu.csr.load(csr_addr);

  // Store the original CSR value into the rd register
  cpu.regs[rd] = t;

  // Set the CSR register value to the immediate value
  cpu.csr.store(csr_addr, rs1);

  // Update the program counter
  return cpu.update_pc();
}

std::optional<uint64_t> executeCSR_RSI(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto csr_addr = (inst & 0xfff00000) >> 20;

  // Load the value from the CSR register
  uint64_t t = cpu.csr.load(csr_addr);

  // Store the original CSR value into the rd register
  cpu.regs[rd] = t;

  // Perform bitwise OR operation between the CSR register value and the immediate value
  // and store the result back into the CSR register
  cpu.csr.store(csr_addr, t | (1 << rs1));

  // Update the program counter
  return cpu.update_pc();
}

std::optional<uint64_t> executeCSR_RCI(Cpu& cpu, uint32_t inst) {
  auto [rd, rs1, rs2] = unpackInstruction(inst);
  auto csr_addr = (inst & 0xfff00000) >> 20;

  // Load the value from the CSR register
  uint64_t t = cpu.csr.load(csr_addr);

  // Store the original CSR value into the rd register
  cpu.regs[rd] = t;

  // Perform bitwise AND operation between the CSR register value and the bitwise NOT of the immediate value
  // and store the result back into the CSR register
  cpu.csr.store(csr_addr, t & ~rs1);

  // Update the program counter
  return cpu.update_pc();
}

std::optional<uint64_t> executeSFENCE_VMA(Cpu& cpu, uint32_t inst) {
  // 在这里，模拟器没有实现虚拟内存或者页表，
  // 所以这个指令不需要执行任何操作。
  // 如果你的模拟器实现了虚拟内存或者页表，你需要在这里添加适当的代码来刷新TLB。

  // 更新程序计数器
  return cpu.update_pc();
}

std::optional<uint64_t> executeSRET(Cpu& cpu, uint32_t inst) {
  // 从 CSR 寄存器加载 sstatus 的值
  uint64_t sstatus = cpu.csr.load(SSTATUS);

  // 根据 SPP 位设置权限级别，SPP 位是 sstatus 的第 8 位
  cpu.mode = (sstatus & MASK_SPP) >> 8;

  // SPIE 位是 sstatus 的第 5 位
  uint64_t spie = (sstatus & MASK_SPIE) >> 5;

  // 将 SIE 位设置为 SPIE 位的值，SIE 位是 sstatus 的第 1 位
  sstatus = (sstatus & ~MASK_SIE) | (spie << 1);

  // 将 SPIE 位设置为 1
  sstatus |= MASK_SPIE;

  // 将 SPP 位设置为最低权限模式（U-mode）
  sstatus &= ~MASK_SPP;

  // 将修改后的 sstatus 值存回 sstatus 寄存器
  cpu.csr.store(SSTATUS, sstatus);

  // 将程序计数器（PC）设置为 sepc 寄存器的值
  // 当 IALIGN=32 时，sepc[1] 位在读取时被屏蔽，使其看起来像是 0。这种屏蔽也发生在 SRET 指令的隐式读取中
  uint64_t new_pc = cpu.csr.load(SEPC) & ~0b11;

  // 返回新的程序计数器（PC）的值
  return new_pc;
}

std::optional<uint64_t> executeMRET(Cpu& cpu, uint32_t inst) {
  // Load the value of the mstatus register from the CSR
  uint64_t mstatus = cpu.csr.load(MSTATUS);

  // Set the privilege level based on the MPP bit, which is the 11th and 12th bits of mstatus
  cpu.mode = (mstatus & MASK_MPP) >> 11;

  // The MPIE bit is the 7th bit of mstatus
  uint64_t mpie = (mstatus & MASK_MPIE) >> 7;

  // Set the MIE bit to the value of the MPIE bit, the MIE bit is the 3rd bit of mstatus
  mstatus = (mstatus & ~MASK_MIE) | (mpie << 3);

  // Set the MPIE bit to 1
  mstatus |= MASK_MPIE;

  // Set the MPP bit to the least privilege mode (U-mode)
  mstatus &= ~MASK_MPP;

  // If MPP is not equal to M, set the MPRV bit to 0
  if ((mstatus & MASK_MPP) != Machine) {
    mstatus &= ~MASK_MPRV;
  }

  // Store the modified mstatus value back to the mstatus register
  cpu.csr.store(MSTATUS, mstatus);

  // Set the program counter (PC) to the value of the mepc register
  // When IALIGN=32, the mepc[1] bit is masked when read, making it look like 0. This masking also occurs in the implicit read of the MRET instruction
  uint64_t new_pc = cpu.csr.load(MEPC) & ~0b11;

  // Return the new value of the program counter (PC)
  return new_pc;
}

std::optional<uint64_t> executeBNE(Cpu& cpu, uint32_t inst) {
    auto [rd, rs1, rs2] = unpackInstruction(inst);
    auto imm = static_cast<int64_t>((((inst & 0x80000000) ? 0xFFF00000 : 0) |
                                     ((inst & 0x80) << 4) |
                                     ((inst >> 20) & 0x7E0) |
                                     ((inst >> 7) & 0x1E)) << 1) >> 1;  // Sign-extend

    if (cpu.regs[rs1] != cpu.regs[rs2]) {
        LOG(INFO, "BNE: pc = pc + ", imm);
        return cpu.pc + imm;
    }

    return std::nullopt;
}

/**
 * @brief Executes the BLT (Branch if Less Than) instruction in the RISC-V instruction set.
 *
 * The BLT instruction compares two registers and, if the first is less than the second,
 * branches to an instruction located a certain distance away.
 *
 * @param cpu The current state of the CPU, including registers and other relevant data.
 * @param inst The 32-bit binary representation of the instruction to execute.
 *
 * @return If the branch is taken, returns the new program counter value. If the branch is not taken, returns std::nullopt.
 */
std::optional<uint64_t> executeBLT(Cpu& cpu, uint32_t inst) {
  // Unpack the instruction into its constituent parts
  auto [rd, rs1, rs2] = unpackInstruction(inst);

  // Calculate the branch offset, taking into account that it's a signed value
  int64_t imm = static_cast<int64_t>((static_cast<int64_t>(inst) & 0x80000000 ? 0xFFFFF000LL : 0) |
                ((static_cast<uint64_t>(inst) & 0x80) << 4) |
                ((inst >> 20) & 0x7E0) |
                ((inst >> 7) & 0x1E));

  // If the value in rs1 is less than the value in rs2, branch to the calculated offset
  if (static_cast<int64_t>(cpu.regs[rs1]) < static_cast<int64_t>(cpu.regs[rs2])) {
    LOG(INFO, "BLT: pc = pc + ", imm);
    return cpu.pc + imm;
  }

  // If the branch is not taken, return std::nullopt
  return std::nullopt;
}


std::optional<uint64_t> executeBGE(Cpu& cpu, uint32_t inst) {
    auto [rd, rs1, rs2] = unpackInstruction(inst);
    auto imm = static_cast<int64_t>((((inst & 0x80000000) ? 0xFFF00000 : 0) |
                                     ((inst & 0x80) << 4) |
                                     ((inst >> 20) & 0x7E0) |
                                     ((inst >> 7) & 0x1E)) << 1) >> 1;  // Sign-extend

    if (static_cast<int64_t>(cpu.regs[rs1]) >= static_cast<int64_t>(cpu.regs[rs2])) {
        LOG(INFO, "BGE: pc = pc + ", imm);
        return cpu.pc + imm;
    }

    return std::nullopt;
}

std::optional<uint64_t> executeBGEU(Cpu& cpu, uint32_t inst) {
    auto [rd, rs1, rs2] = unpackInstruction(inst);
    auto imm = static_cast<int64_t>((((inst & 0x80000000) ? 0xFFF00000 : 0) |
                                     ((inst & 0x80) << 4) |
                                     ((inst >> 20) & 0x7E0) |
                                     ((inst >> 7) & 0x1E)) << 1) >> 1;  // Sign-extend

    if (cpu.regs[rs1] >= cpu.regs[rs2]) {
        LOG(INFO, "BGEU: pc = pc + ", imm);
        return cpu.pc + imm;
    }

    return std::nullopt;
}

using ExecuteFunction = std::function<std::optional<uint64_t>(Cpu&, uint32_t)>;

std::optional<uint64_t> InstructionExecutor::execute(Cpu& cpu, uint32_t inst) {
  uint32_t opcode = inst & 0x0000007f;
  uint32_t funct3  = (inst & 0x00007000) >> 12;
  uint32_t funct7 = (inst & 0xfe000000) >> 25;

  // x0 is hardwired zero
  cpu.regs[0] = 0;
  LOG(INFO, "Instruction: 0x", std::hex, inst, std::dec);
  LOG(INFO, "Executing instruction with opcode: 0x", std::hex, opcode, std::dec);

  std::unordered_map<uint32_t, ExecuteFunction> opcodeTable = {
    {0x17, executeAUIPC},
    {0x37, executeLui},
    {0x67, executeJALR},
    {0x6f, executeJAL},
  };

  // Look up the corresponding execute function in the opcodeTable
  auto executeFunc = opcodeTable.find(opcode);

  // If found, call the execute function with the given instruction
  if (executeFunc != opcodeTable.end()) {
    auto result = executeFunc->second(cpu, inst);
    if (result.has_value()) {
      LOG(INFO, "Instruction executed successfully. New PC: 0x", std::hex, result.value(), std::dec);
    } else {
      throw Exception(ExceptionType::IllegalInstruction, inst);
    }
    return result;
  }

  LOG(INFO, "funct3: 0x", std::hex, funct3, std::dec);

  std::unordered_map<std::tuple<uint32_t, uint32_t>, ExecuteFunction> instructionMap = {
    {std::make_tuple(0x03, 0x0), executeLb},
    {std::make_tuple(0x03, 0x1), executeLh},
    {std::make_tuple(0x03, 0x2), executeLw},
    {std::make_tuple(0x03, 0x3), executeLd},
    {std::make_tuple(0x03, 0x4), executeLbu},
    {std::make_tuple(0x03, 0x5), executeLhu},
    {std::make_tuple(0x03, 0x6), executeLwu},
    {std::make_tuple(0x0f, 0x0), executeFence},
    {std::make_tuple(0x13, 0x0), executeAddi},
    {std::make_tuple(0x13, 0x1), executeSlli},
    {std::make_tuple(0x13, 0x2), executeSlti},
    {std::make_tuple(0x13, 0x3), executeSltiu},
    {std::make_tuple(0x13, 0x4), executeXori},
    {std::make_tuple(0x13, 0x6), executeOri},
    {std::make_tuple(0x13, 0x7), executeAndi},
    {std::make_tuple(0x19, 0x7), executeSb},
    {std::make_tuple(0x23, 0x0), executeStoreByte},
    {std::make_tuple(0x23, 0x3), executeStoreDouble},
    {std::make_tuple(0x63, 0x1), executeBEQ},
    {std::make_tuple(0x73, 0x1), executeCSR_RW},
    {std::make_tuple(0x73, 0x2), executeCSR_RS},
    {std::make_tuple(0x73, 0x3), executeCSR_RC},
    {std::make_tuple(0x73, 0x5), executeCSR_RWI},
    {std::make_tuple(0x73, 0x6), executeCSR_RSI},
    {std::make_tuple(0x73, 0x7), executeCSR_RCI},
  };

  auto it = instructionMap.find({opcode, funct3});
  if (it != instructionMap.end()) {
    auto result = it->second(cpu, inst);
    if (result.has_value()) {
      LOG(INFO, "Instruction executed successfully. New PC: 0x", std::hex, result.value(), std::dec);
    } else {
      throw Exception(ExceptionType::IllegalInstruction, inst);
    }
    return result;
  }

  LOG(INFO, "funct7: 0x", std::hex, funct7, std::dec);

  std::unordered_map<std::tuple<uint32_t, uint32_t, uint32_t>, ExecuteFunction> instruction2Map = {
    {std::make_tuple(0x13, 0x5, 0x00), executeSrli},
    {std::make_tuple(0x13, 0x5, 0x20), executeSrai},
    {std::make_tuple(0x33, 0x0, 0x00), executeAdd},
    {std::make_tuple(0x33, 0x1, 0x00), executeSll},
    {std::make_tuple(0x33, 0x2, 0x00), executeSlt},
    {std::make_tuple(0x33, 0x4, 0x00), executeXor},
    {std::make_tuple(0x33, 0x5, 0x00), executeSrl},
    {std::make_tuple(0x33, 0x5, 0x20), executeSra},
    {std::make_tuple(0x33, 0x6, 0x00), executeOr},
    {std::make_tuple(0x33, 0x7, 0x00), executeAnd},
    {std::make_tuple(0x3b, 0x0, 0x00), executeAddw},
    {std::make_tuple(0x73, 0x0, 0x9), executeSFENCE_VMA},
    {std::make_tuple(0x73, 0x0, 0x8), executeSRET},
    {std::make_tuple(0x73, 0x0, 0x18), executeMRET},
  };

  auto it1 = instruction2Map.find({opcode, funct3, funct7});
  LOG(INFO, "Executing srli or srai funct7: 0x" , std::hex, funct7 , std::dec);
  if (it1 != instruction2Map.end()) {
    auto result = it1->second(cpu, inst);
    if (result.has_value()) {
      LOG(INFO, "Instruction executed successfully. New PC: 0x", std::hex, result.value(), std::dec);
    } else {
      throw Exception(ExceptionType::IllegalInstruction, inst);
    }
    return result;
  } else {
    LOG(ERROR, "Unsupported instruction: 0x", std::hex, inst,
      ", opcode: 0x", opcode, ", funct3: 0x", funct3,
      ", funct7: 0x", std::hex, funct7, std::dec);
    throw Exception(ExceptionType::IllegalInstruction, inst);
  }
}

}