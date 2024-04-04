//
// Created by Jie Wei on 2024/4/2.
//

#include <gtest/gtest.h>
#include "../../src/exception.h"

namespace cemu {

TEST(ExceptionTest, Constructor) {
  Exception e( ExceptionType::InstructionAddrMisaligned , 0x1234 );
  EXPECT_EQ( e.getType(), ExceptionType::InstructionAddrMisaligned );
  EXPECT_EQ( e.getValue(), 0x1234 );
}

TEST(ExceptionTest, GetValueAndGetType) {
  Exception e(ExceptionType::Breakpoint, 0x5678);
  EXPECT_EQ(e.getType(), ExceptionType::Breakpoint);
  EXPECT_EQ(e.getValue(), 0x5678);
}

TEST(ExceptionTest, IsFatal) {
  Exception e1(ExceptionType::InstructionAddrMisaligned, 0);
  EXPECT_TRUE(e1.isFatal());

  Exception e2(ExceptionType::Breakpoint, 0);
  EXPECT_FALSE(e2.isFatal());
}

TEST(ExceptionTest, PrintException) {
  Exception e(ExceptionType::InstructionAddrMisaligned, 0x1234);
  std::ostringstream os;
  os << e;
  EXPECT_EQ(os.str(), "Instruction address misaligned 1234");
}

}