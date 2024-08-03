#include "cpu.h"
#include "test_framework.h"

std::string tokenTypeToString(int tokenType) {
  return std::to_string(tokenType);
}

void testCpuInitialization() {
  std::cout << "Starting testCpuInitialization...\n";

  std::vector<uint8_t> code = {0x01, 0x02, 0x03, 0x04};
  Cpu cpu(code);

  std::cout << "Testing CPU Register Initialization...\n";
  // Check if registers are initialized correctly
  TestFramework::assertEqual("Test CPU Register 0",
                             std::to_string(cpu.getReg(0)), std::to_string(0));
  TestFramework::assertEqual("Test CPU Register 2",
                             std::to_string(cpu.getReg(2)),
                             std::to_string(DRAM_SIZE - 1));
  std::cout << "Passed CPU Register Initialization Tests.\n";

  std::cout << "Testing Memory Initialization...\n";
  // Check if memory is loaded with the code correctly
  TestFramework::assertEqual("Test Memory Byte 0",
                             std::to_string(cpu.getMem(0)),
                             std::to_string(0x01));
  TestFramework::assertEqual("Test Memory Byte 1",
                             std::to_string(cpu.getMem(1)),
                             std::to_string(0x02));
  TestFramework::assertEqual("Test Memory Byte 2",
                             std::to_string(cpu.getMem(2)),
                             std::to_string(0x03));
  TestFramework::assertEqual("Test Memory Byte 3",
                             std::to_string(cpu.getMem(3)),
                             std::to_string(0x04));
  TestFramework::assertEqual("Test Memory Byte 4",
                             std::to_string(cpu.getMem(4)),
                             std::to_string(0));  // Should be zero
  std::cout << "Passed Memory Initialization Tests.\n";
}

int main() {
  std::cout << "Starting CPU tests...\n";
  testCpuInitialization();
  std::cout << "All tests completed successfully.\n";
  return 0;
}
