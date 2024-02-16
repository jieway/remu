//
// Created by Jie Wei on 2024/2/13.
//

#include "csr.h"

namespace crvemu {

/**
 * @brief Constructor for the Csr class.
 *
 * This constructor initializes all CSR (Control Status Registers) to 0.
 */
Csr::Csr() {
  csrs.fill(0); // 初始化所有CSR为0
}


/**
 * @brief Prints the current state of the control status registers.
 *
 * This function prints the current state of the control status registers in a formatted manner.
 * It prints the values of MSTATUS, MTVEC, MEPC, MCAUSE, SSTATUS, STVEC, SEPC, and SCAUSE registers.
 */
void Csr::dump_csrs() const {
  std::cout << std::setw(80) << std::setfill('-') << "control status registers" << std::setfill(' ') << "\n";
  std::cout << "mstatus = " << std::hex << std::setw(18) << load(MSTATUS)
            << "  mtvec = " << std::setw(18) << load(MTVEC)
            << "  mepc = " << std::setw(18) << load(MEPC)
            << "  mcause = " << std::setw(18) << load(MCAUSE) << "\n";
  std::cout << "sstatus = " << std::setw(18) << load(SSTATUS)
            << "  stvec = " << std::setw(18) << load(STVEC)
            << "  sepc = " << std::setw(18) << load(SEPC)
            << "  scause = " << std::setw(18) << load(SCAUSE) << "\n";
}

/**
 * @brief Loads the value of a specific CSR.
 *
 * This function loads the value of a specific CSR from the array of CSRs.
 * It returns the value of the CSR at the specified address.
 *
 * @param addr The address of the CSR to load.
 * @return The value of the CSR at the specified address.
 */
uint64_t Csr::load(size_t addr) const {
  switch (addr) {
    case SIE:
      return csrs[MIE] & csrs[MIDELEG];
    case SIP:
      return csrs[MIP] & csrs[MIDELEG];
    case SSTATUS:
      return csrs[MSTATUS] & MASK_SSTATUS;
    default:
      return csrs[addr];
  }
}

/**
 * @brief Stores a value into a specific CSR.
 *
 * This function stores a value into a specific CSR at the given address.
 * Depending on the address, the function performs different operations:
 * - If the address is SIE, it stores the value into the MIE register, taking into account the MIDELEG register.
 * - If the address is SIP, it stores the value into the MIP register, taking into account the MIDELEG register.
 * - If the address is SSTATUS, it stores the value into the MSTATUS register, taking into account the MASK_SSTATUS.
 * - For any other address, it directly stores the value into the CSR at the given address.
 *
 * @param addr The address of the CSR to store the value into.
 * @param value The value to store into the CSR.
 */
void Csr::store(size_t addr, uint64_t value) {
  switch (addr) {
    case SIE:
      csrs[MIE] = (csrs[MIE] & ~csrs[MIDELEG]) | (value & csrs[MIDELEG]);
    break;
    case SIP:
      csrs[MIP] = (csrs[MIE] & ~csrs[MIDELEG]) | (value & csrs[MIDELEG]);
    break;
    case SSTATUS:
      csrs[MSTATUS] = (csrs[MSTATUS] & ~MASK_SSTATUS) | (value & MASK_SSTATUS);
    break;
    default:
      csrs[addr] = value;
  }
}

/**
 * @brief Checks if a specific bit in the MEDELEG register is set.
 *
 * This function checks if the bit at the position specified by 'val' in the MEDELEG register is set.
 * The MEDELEG register is used to delegate some exceptions from M-mode to S-mode.
 * If the bit is set, it means that the corresponding exception is delegated.
 *
 * @param val The position of the bit to check in the MEDELEG register.
 * @return true if the bit at position 'val' is set, false otherwise.
 */
bool Csr::is_medelegated(uint64_t val) const {
  return (csrs[MEDELEG] >> val & 1) == 1;
}

/**
 * @brief Checks if a specific bit in the MIDELEG register is set.
 *
 * This function checks if the bit at the position specified by 'val' in the MIDELEG register is set.
 * The MIDELEG register is used to delegate some interrupts from M-mode to S-mode.
 * If the bit is set, it means that the corresponding interrupt is delegated.
 *
 * @param val The position of the bit to check in the MIDELEG register.
 * @return true if the bit at position 'val' is set, false otherwise.
 */
bool Csr::is_midelegated(uint64_t cause) const {
  return (csrs[MIDELEG] >> cause & 1) == 1;
}

}