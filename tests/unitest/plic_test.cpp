//
// Created by Jie Wei on 2024/4/2.
//

#include "gtest/gtest.h"
#include "../../src/plic.h"
#include "../../src/param.h"

namespace cemu {

class PlicTest : public ::testing::Test {
 protected:
  Plic plic;
};

TEST_F(PlicTest, LoadStoreTest) {
  uint64_t test_value = 123;

  // Test store and load for each register
  plic.store(PLIC_PENDING, 32, test_value);
  EXPECT_EQ(plic.load(PLIC_PENDING, 32), test_value);

  plic.store(PLIC_SENABLE, 32, test_value);
  EXPECT_EQ(plic.load(PLIC_SENABLE, 32), test_value);

  plic.store(PLIC_SPRIORITY, 32, test_value);
  EXPECT_EQ(plic.load(PLIC_SPRIORITY, 32), test_value);

  plic.store(PLIC_SCLAIM, 32, test_value);
  EXPECT_EQ(plic.load(PLIC_SCLAIM, 32), test_value);
}

// Test exception throwing when size is not 32
TEST_F(PlicTest, ExceptionTest) {
  EXPECT_THROW(plic.load(PLIC_PENDING, 64), Exception);
  EXPECT_THROW(plic.store(PLIC_PENDING, 64, 123), Exception);
}

// 测试未知寄存器地址的行为
TEST_F(PlicTest, UnknownAddressTest) {
  uint64_t unknown_address = 0xdeadbeef;
  EXPECT_NO_THROW(plic.store(unknown_address, 32, 123));
  EXPECT_EQ(plic.load(unknown_address, 32), 0);
}

//// 测试处理多个中断请求的行为
//TEST_F(PlicTest, MultipleInterruptsTest) {
//  plic.store(PLIC_PENDING, 32, 1);
//  plic.store(PLIC_PENDING, 32, 2);
//  EXPECT_EQ(plic.load(PLIC_SCLAIM, 32), 1);
//  EXPECT_EQ(plic.load(PLIC_SCLAIM, 32), 2);
//}

//// 测试处理优先级不同的中断请求的行为
//TEST_F(PlicTest, PriorityTest) {
//  plic.store(PLIC_PENDING, 32, 1);
//  plic.store(PLIC_SPRIORITY, 32, 1);
//  plic.store(PLIC_PENDING, 32, 2);
//  plic.store(PLIC_SPRIORITY, 32, 2);
//  EXPECT_EQ(plic.load(PLIC_SCLAIM, 32), 2);
//  EXPECT_EQ(plic.load(PLIC_SCLAIM, 32), 1);
//}
}  // namespace cemu