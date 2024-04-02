// clint_test.cpp

#include "gtest/gtest.h"
#include "../../src/clint.h"

namespace crvemu {

class ClintTest : public ::testing::Test {
 protected:
  Clint clint;
};

TEST_F(ClintTest, ClintConstructorTest) {
  EXPECT_EQ(clint.load(CLINT_MTIME, 64), 0);
  EXPECT_EQ(clint.load(CLINT_MTIMECMP, 64), 0);
}

TEST_F(ClintTest, ClintStoreLoadTest) {
  uint64_t test_value = 123456789;
  clint.store(CLINT_MTIME, 64, test_value);
  EXPECT_EQ(clint.load(CLINT_MTIME, 64), test_value);
  clint.store(CLINT_MTIMECMP, 64, test_value);
  EXPECT_EQ(clint.load(CLINT_MTIMECMP, 64), test_value);
}

TEST_F(ClintTest, ClintInvalidSizeTest) {
  EXPECT_THROW(clint.load(CLINT_MTIME, 32), Exception);
  EXPECT_THROW(clint.store(CLINT_MTIME, 32, 0), Exception);
}

TEST_F(ClintTest, ClintInvalidAddressTest) {
  uint64_t invalid_address = 0xdeadbeef;
  EXPECT_THROW(clint.load(invalid_address, 64), Exception);
  EXPECT_THROW(clint.store(invalid_address, 64, 0), Exception);
}

}