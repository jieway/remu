#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <iostream>
#include "../src/cup.h"
#include "test_util.h"

namespace crvemu {
    // TEST(RVTests, TestLb) {
    //     std::string code = start +
    //         "addi x2, x0, 0 \n"      // Load 0 into x2
    //         "addi x1, x0, 10 \n"     // Load 10 into x1
    //         "sb x1, 5(x2) \n"        // Store 10 at address x2 + 5
    //         "lb x3, 5(x2) \n";       // Load signed byte from address x2 + 5 into x3
    //
    //     Cpu cpu = rv_helper(code, "test_lb", 3);
    //     EXPECT_EQ(cpu.regs[3], 10) << "Error: x3 should be the result of LB instruction";
    // }
  // 测试 Dram::load 方法
  // TEST(DramTest, LoadTest) {
  //   Dram dram;
  //
  //   // 测试非法大小
  //   EXPECT_EQ(dram.load(0x1000, 7), std::nullopt);
  //
  //   // 假设 DRAM_BASE 为 0x1000，DRAM 大小为 16 字节
  //   // 测试地址超出范围
  //   EXPECT_EQ(dram.load(0x1010, 16), std::nullopt);
  //
  //   // 测试正常加载
  //   // dram.store(0x1000, 0x1122334455667788);
  //   // EXPECT_EQ(dram.load(0x1000, 64), 0x1122334455667788);
  // }

    // Test addi instruction
    TEST(RVTests, TestAddi) {
        std::string code = start +
            "addi x31, x0, 42 \n";
        Cpu cpu = rv_helper(code, "test_addi", 1);
        EXPECT_EQ(cpu.regs[31], 42) << "Error: x31 should be 42 after ADDI instruction";
    }

    // Test slli instruction
    TEST(RVTests, TestSlli) {
        std::string code = start +
            "addi x2, x0, 5 \n"    // Load 5 into x2
            "slli x1, x2, 3 \n";   // x1 = x2 << 3
        Cpu cpu = rv_helper(code, "test_slli", 2);

        // Verify if x1 has the correct value
        EXPECT_EQ(cpu.regs[1], 5 << 3) << "Error: x1 should be the result of SLLI instruction";
    }

    // Test slti instruction
    TEST(RVTests, TestSlti) {
        std::string code = start +
            "addi x2, x0, 8 \n"    // 将 8 加载到 x2 中
            "slti x1, x2, 10 \n";  // x1 = (x2 < 10) ? 1 : 0
        Cpu cpu = rv_helper(code, "test_slti", 2);

        // 验证 x1 的值是否正确
        EXPECT_EQ(cpu.regs[1], 1) << "Error: x1 should be the result of SLTI instruction";
    }

    // Test sltiu instruction
    TEST(RVTests, TestSltiu) {
        std::string code = start +
            "addi x2, x0, 8 \n"      // 将 8 加载到 x2 中
            "sltiu x1, x2, 10 \n";   // x1 = (x2 < 10) ? 1 : 0 (unsigned comparison)
        Cpu cpu = rv_helper(code, "test_sltiu", 2);

        // 验证 x1 的值是否正确
        EXPECT_EQ(cpu.regs[1], 1) << "Error: x1 should be the result of SLTIU instruction";
    }

    // Test xori instruction
    TEST(RVTests, TestXori) {
        std::string code = start +
            "addi x2, x0, 15 \n"      // Load 15 into x2
            "xori x1, x2, 7 \n";      // x1 = x2 ^ 7
        Cpu cpu = rv_helper(code, "test_xori", 2);

        // Verify if x1 has the correct value
        EXPECT_EQ(cpu.regs[1], 8) << "Error: x1 should be the result of XORI instruction";
    }

    // Test srli instruction
    TEST(RVTests, TestSrli) {
        std::string code = start +
            "addi x2, x0, 32 \n"     // Load 32 into x2
            "srli x1, x2, 3 \n";     // x1 = x2 >> 3 (logical right shift)
        Cpu cpu = rv_helper(code, "test_srli", 2);

        // Verify if x1 has the correct value
        EXPECT_EQ(cpu.regs[1], 4) << "Error: x1 should be the result of SRLI instruction";
    }

    // Test srai instruction
    // TEST(RVTests, TestSrai) {
    //     std::string code = start +
    //         "addi x2, x0, -16 \n"    // Load -16 into x2
    //         "srai x1, x2, 3 \n";     // x1 = x2 >> 3 (arithmetic right shift)
    //     Cpu cpu = rv_helper(code, "test_srai", 2);
    //
    //     // Verify if x1 has the correct value
    //     EXPECT_EQ(cpu.regs[1], static_cast<uint64_t>(-2)) << "Error: x1 should be the result of SRAI instruction";
    // }

    // Test add instruction
    TEST(RVTests, TestAdd) {
        std::string code = start +
            "addi x2, x0, 10 \n"   // 将 10 加载到 x2 中
            "addi x3, x0, 20 \n"   // 将 20 加载到 x3 中
            "add x1, x2, x3 \n";  // x1 = x2 + x3
        Cpu cpu = rv_helper(code, "test_add", 3);

        // 验证 x1 的值是否正确
        EXPECT_EQ(cpu.regs[1], 30) << "Error: x1 should be the result of ADD instruction";
    }

    // Test ori instruction
    TEST(RVTests, TestOri) {
        std::string code = start +
            "addi x2, x0, 15 \n"      // Load 15 into x2
            "ori x1, x2, 7 \n";       // x1 = x2 | 7
        Cpu cpu = rv_helper(code, "test_ori", 2);

        // Verify if x1 has the correct value
        EXPECT_EQ(cpu.regs[1], 15 | 7) << "Error: x1 should be the result of ORI instruction";
    }

    // Test andi instruction
    TEST(RVTests, TestAndi) {
        std::string code = start +
            "addi x2, x0, 15 \n"      // Load 15 into x2
            "andi x1, x2, 7 \n";      // x1 = x2 & 7
        Cpu cpu = rv_helper(code, "test_andi", 2);

        // Verify if x1 has the correct value
        EXPECT_EQ(cpu.regs[1], 15 & 7) << "Error: x1 should be the result of ANDI instruction";
    }
}