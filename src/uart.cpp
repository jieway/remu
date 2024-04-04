// uart.cpp

#include "uart.h"
#include <iostream>
#include "log.h"

namespace cemu {

Uart::Uart() : uart(UART_SIZE), interrupt(false) {
  uart[UART_LSR] |= MASK_UART_LSR_TX;
  std::thread([this]() {
    char byte;
    while (std::cin >> byte) {
      std::unique_lock<std::mutex> lock(mtx);
      while ((uart[UART_LSR] & MASK_UART_LSR_RX) == 1) {
        cv.wait(lock);
      }
      uart[UART_RHR] = byte;
      interrupt.store(true);
      uart[UART_LSR] |= MASK_UART_LSR_RX;
    }
  }).detach();
}

bool Uart::is_interrupting() {
  return interrupt.exchange(false);
}

uint64_t Uart::load(uint64_t addr, uint64_t size) {
  if (size != 8) {
    throw Exception(ExceptionType::LoadAccessFault, addr);
  }
  std::lock_guard<std::mutex> lock(mtx);
  uint64_t index = addr - UART_BASE;
  if (index == UART_RHR) {
    cv.notify_one();
    uart[UART_LSR] &= ~MASK_UART_LSR_RX;
    return uart[UART_RHR];
  } else {
    return uart[index];
  }
}

void Uart::store(uint64_t addr, uint64_t size, uint64_t value) {
  if (size != 8) {
    throw Exception(ExceptionType::StoreAMOAccessFault, addr);
  }
  std::lock_guard<std::mutex> lock(mtx);
  uint64_t index = addr - UART_BASE;
  if (index == UART_THR) {
    std::cout << static_cast<char>(value);
    std::cout.flush();
  } else {
    uart[index] = value;
  }
}

}