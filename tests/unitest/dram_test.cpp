#include <gtest/gtest.h>
#include "../../src/dram.h"
#include "../../src/param.h"
#include "../../src/exception.h"

namespace cemu {

class DramTest : public ::testing::Test {
protected:
  std::vector<uint8_t> code = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  Dram dram = Dram(code);
};

TEST_F(DramTest, LoadTest) {
  auto value = dram.load(DRAM_BASE, 64);
  ASSERT_TRUE(value.has_value());
  ASSERT_EQ(value.value(), 0x0807060504030201);
}

TEST_F(DramTest, StoreTest) {
  uint64_t store_value = 0x0102030405060708;
  ASSERT_TRUE(dram.store(DRAM_BASE, 64, store_value));

  auto load_value = dram.load(DRAM_BASE, 64);
  ASSERT_TRUE(load_value.has_value());
  ASSERT_EQ(load_value.value(), store_value);
}

TEST_F(DramTest, InvalidLoadTest) {
  EXPECT_THROW(dram.load(DRAM_BASE, 10), Exception);
}

TEST_F(DramTest, InvalidStoreTest) {
  EXPECT_THROW(dram.store(DRAM_BASE, 10, 0x01), Exception);
}

}  // namespace cemu