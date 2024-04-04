#include <gtest/gtest.h>
#include "../../src/bus.h"
#include "../../src/param.h"
#include "../../src/exception.h"

namespace cemu {

class BusTest : public ::testing::Test {
protected:
  std::vector<uint8_t> code = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  Bus bus = Bus(code);
};

TEST_F(BusTest, LoadTest) {
  auto value = bus.load(DRAM_BASE, 64);
  ASSERT_TRUE(value.has_value());
  ASSERT_EQ(value.value(), 0x0807060504030201);
}

TEST_F(BusTest, StoreTest) {
  uint64_t store_value = 0x0102030405060708;
  ASSERT_TRUE(bus.store(DRAM_BASE, 64, store_value));

  auto load_value = bus.load(DRAM_BASE, 64);
  ASSERT_TRUE(load_value.has_value());
  ASSERT_EQ(load_value.value(), store_value);
}

TEST_F(BusTest, InvalidLoadTest) {
  EXPECT_THROW(bus.load(DRAM_BASE, 10), Exception);
}

TEST_F(BusTest, InvalidStoreTest) {
  EXPECT_THROW(bus.store(DRAM_BASE, 10, 0x01), Exception);
}

}  // namespace cemu