// Dram.h
#ifndef DRAM_H
#define DRAM_H

#include <vector>
#include <cstdint>

class Dram {
public:
    Dram(const std::vector<uint8_t>& code);

    uint64_t load(uint64_t addr, uint64_t size);
    void store(uint64_t addr, uint64_t size, uint64_t value);

private:
    std::vector<uint8_t> dram;
};

#endif // DRAM_H
