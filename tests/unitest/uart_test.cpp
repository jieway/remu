// uart_test.cpp

#include "gtest/gtest.h"
#include "../../src/uart.h"

namespace cemu {

class UartTest : public ::testing::Test {
 protected:
  Uart uart;
};

TEST_F(UartTest, IsInterrupting) {
  EXPECT_EQ(uart.is_interrupting(), false);
}

TEST_F(UartTest, LoadAndStore) {
  uint64_t addr = UART_BASE + 1;
  uint64_t size = 8;
  uint64_t value = 0;
  uint64_t expected_value = 123;

  uart.store(addr, size, expected_value);
  value = uart.load(addr, size);

  EXPECT_EQ(value, expected_value);
}

TEST_F(UartTest, LoadClearsInterruptFlagAndLSRRXBit) {
  uint64_t addr = UART_BASE + UART_RHR;
  uint64_t size = 8;

  uart.load(addr, size);

  EXPECT_EQ(uart.is_interrupting(), false);
  EXPECT_EQ((uart.load(UART_BASE + UART_LSR, size) & MASK_UART_LSR_RX), 0);
}

TEST_F(UartTest, LoadAccessFault) {
  uint64_t addr = UART_BASE;
  uint64_t size = 4;

  EXPECT_THROW(uart.load(addr, size), Exception);
}

TEST_F(UartTest, StoreAMOAccessFault) {
  uint64_t addr = UART_BASE;
  uint64_t size = 4;
  uint64_t value = 123;

  EXPECT_THROW(uart.store(addr, size, value), Exception);
}

}  // namespace cemu