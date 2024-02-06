// //
// // Created by Jie Wei on 2024/2/7.
// //
//
// #include "gtest/gtest.h"
// #include "../src/dram.h"
// #include "../src/exception.h"
// #include "../src/param.h"
//
// class DramTest : public ::testing::Test {
// protected:
//     Dram dram;
//
//     void SetUp() override {
//         std::vector<uint8_t> code = {0x12, 0x34, 0x56, 0x78};
//         dram = Dram(code);
//     }
// };
//
// // Test loading a value
// TEST_F(DramTest, Load) {
//     uint64_t value = dram.load(DRAM_BASE, 8);
//     EXPECT_EQ(value, 0x12345678);
// }
//
// // Test storing a value
// TEST_F(DramTest, Store) {
//     dram.store(DRAM_BASE, 8, 0xABCD1234);
//     uint64_t value = dram.load(DRAM_BASE, 8);
//     EXPECT_EQ(value, 0xABCD1234);
// }
//
// // Test loading with invalid size
// TEST_F(DramTest, LoadInvalidSize) {
//     EXPECT_THROW(dram.load(DRAM_BASE, 4), Exception);
// }
//
// // Test storing with invalid size
// TEST_F(DramTest, StoreInvalidSize) {
//     EXPECT_THROW(dram.store(DRAM_BASE, 4, 0x1234), Exception);
// }
//
// // Test loading from out-of-range address
// TEST_F(DramTest, LoadOutOfRange) {
//     EXPECT_THROW(dram.load(DRAM_BASE + DRAM_SIZE, 8), std::out_of_range);
// }
//
// // Test storing to out-of-range address
// TEST_F(DramTest, StoreOutOfRange) {
//     EXPECT_THROW(dram.store(DRAM_BASE + DRAM_SIZE, 8, 0x1234), std::out_of_range);
// }
