// Bus.cpp
#include "bus.h"
#include "param.h"
#include <stdexcept> // For std::runtime_error
#include <string>
#include "exception.h"
namespace cemu {
    Bus::Bus(const std::vector<uint8_t>& code) : dram(code) {}

    uint64_t Bus::load(uint64_t addr, uint64_t size) {
        try {
            if (addr >= DRAM_BASE && addr <= DRAM_END) {
                return dram.load(addr, size);
            } else {
                throw Exception(Exception::Type::LoadAccessFault, addr);
            }
        } catch (const Exception& e) {
            std::cerr << "Exception Bus::load: " << e << std::endl;
        }
    }

    void Bus::store(uint64_t addr, uint64_t size, uint64_t value) {
        try {
            if (addr >= DRAM_BASE && addr <= DRAM_END) {
                dram.store(addr, size, value);
            } else {
                throw Exception(Exception::Type::StoreAMOAccessFault, addr);
            }
        } catch (const Exception& e) {
            std::cerr << "Exception Bus::store: " << e << std::endl;
        }

    }
}