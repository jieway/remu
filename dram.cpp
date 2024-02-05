// Dram.cpp
#include <stdexcept>
#include "dram.h"

#include "exception.h"
#include "param.h"

Dram::Dram(const std::vector<uint8_t>& code) {
    dram.resize(DRAM_SIZE, 0); // Initialize DRAM with 0s
    // Copy code into dram
    std::copy(code.begin(), code.end(), dram.begin());
}

uint64_t Dram::load(uint64_t addr, uint64_t size) {
    try {
        if (size != 8 && size != 16 && size != 32 && size != 64) {
            throw Exception(Exception::Type::LoadAccessFault, addr);
        }
        uint64_t nbytes = size / 8;
        std::size_t index = (addr - DRAM_BASE);
        if (index + nbytes > dram.size()) {
            throw std::out_of_range("Address out of range");
        }

        uint64_t value = 0;
        for (uint64_t i = 0; i < nbytes; ++i) {
            value |= static_cast<uint64_t>(dram[index + i]) << (i * 8);
        }
        return value;
    } catch (const Exception& e) {
        std::cerr << "Exception Dram::load: " << e << std::endl;
    }
}

void Dram::store(uint64_t addr, uint64_t size, uint64_t value) {
    try {
        if (size != 8 && size != 16 && size != 32 && size != 64) {
            throw Exception(Exception::Type::StoreAMOAccessFault, addr);
        }
        uint64_t nbytes = size / 8;
        std::size_t index = (addr - DRAM_BASE);
        if (index + nbytes > dram.size()) {
            throw std::out_of_range("Address out of range");
        }

        for (uint64_t i = 0; i < nbytes; ++i) {
            dram[index + i] = (value >> (i * 8)) & 0xFF;
        }
    } catch (const Exception& e) {
        std::cerr << "Exception Dram::store: " << e << std::endl;
    }

}
