#include <gtest/gtest.h>
#include "../../src/cup.h"

namespace crvemu {

class CpuTest : public ::testing::Test {
protected:
    std::vector<uint8_t> code = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    Cpu cpu = Cpu(code);
};

TEST_F(CpuTest, LoadTest) {
    auto value = cpu.load(DRAM_BASE, 64);
    ASSERT_TRUE(value.has_value());
    ASSERT_EQ(value.value(), 0x0807060504030201);
}

TEST_F(CpuTest, StoreTest) {
    uint64_t store_value = 0x0102030405060708;
    ASSERT_TRUE(cpu.store(DRAM_BASE, 64, store_value));

    auto load_value = cpu.load(DRAM_BASE, 64);
    ASSERT_TRUE(load_value.has_value());
    ASSERT_EQ(load_value.value(), store_value);
}

TEST_F(CpuTest, InvalidLoadTest) {
    auto value = cpu.load(DRAM_BASE, 10);
    ASSERT_FALSE(value.has_value());
}

TEST_F(CpuTest, InvalidStoreTest) {
    ASSERT_FALSE(cpu.store(DRAM_BASE, 10, 0x01));
}

}  // namespace crvemu