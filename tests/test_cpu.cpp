#include "cpu.h"
#include "test_framework.h"

void testCpuInitialization() {
  std::cout << "Starting testCpuInitialization...\n";

  std::vector<uint8_t> code = {0x01, 0x02, 0x03, 0x04};
  Cpu cpu(code);

  // 测试寄存器初始化
  TestFramework::assertEqual("Test CPU Register 0",
                             std::to_string(cpu.getReg(0)), std::to_string(0));
  TestFramework::assertEqual("Test CPU Register 2",
                             std::to_string(cpu.getReg(2)),
                             std::to_string(DRAM_SIZE - 1));

  // 测试内存加载
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
                             std::to_string(0));  // 应为0

  std::cout << "Passed testCpuInitialization.\n";
}

void testCpuRegisterModification() {
  std::cout << "Starting testCpuRegisterModification...\n";

  std::vector<uint8_t> code = {0x00, 0x00, 0x00, 0x00};  // 空的指令集
  Cpu cpu(code);

  // 修改寄存器并测试
  cpu.setReg(1, 42);
  cpu.setReg(2, 84);

  TestFramework::assertEqual("Test CPU Register 1 after modification",
                             std::to_string(cpu.getReg(1)), std::to_string(42));
  TestFramework::assertEqual("Test CPU Register 2 after modification",
                             std::to_string(cpu.getReg(2)), std::to_string(84));

  std::cout << "Passed testCpuRegisterModification.\n";
}

void testMemoryModification() {
  std::cout << "Starting testMemoryModification...\n";

  std::vector<uint8_t> code = {0x00, 0x00, 0x00, 0x00};  // 空的指令集
  Cpu cpu(code);

  // 修改内存并测试
  cpu.setMem(0, 0xAA);
  cpu.setMem(1, 0xBB);
  cpu.setMem(2, 0xCC);
  cpu.setMem(3, 0xDD);

  TestFramework::assertEqual("Test Memory Byte 0 after modification",
                             std::to_string(cpu.getMem(0)),
                             std::to_string(0xAA));
  TestFramework::assertEqual("Test Memory Byte 1 after modification",
                             std::to_string(cpu.getMem(1)),
                             std::to_string(0xBB));
  TestFramework::assertEqual("Test Memory Byte 2 after modification",
                             std::to_string(cpu.getMem(2)),
                             std::to_string(0xCC));
  TestFramework::assertEqual("Test Memory Byte 3 after modification",
                             std::to_string(cpu.getMem(3)),
                             std::to_string(0xDD));

  std::cout << "Passed testMemoryModification.\n";
}

void testFetchInstruction() {
  std::cout << "Starting testFetchInstruction...\n";

  // 示例指令代码：addi x1, x0, 1
  std::vector<uint8_t> code = {0x93, 0x00, 0x10, 0x00};
  Cpu cpu(code);

  uint32_t inst = cpu.fetch();
  TestFramework::assertEqual("Test Fetch Instruction", std::to_string(inst),
                             std::to_string(0x00100093));

  std::cout << "Passed testFetchInstruction.\n";
}

void testExecuteAddiInstruction() {
  std::cout << "Starting testExecuteAddiInstruction...\n";

  // 示例指令代码：addi x1, x0, 1
  std::vector<uint8_t> code = {0x93, 0x00, 0x10, 0x00};
  Cpu cpu(code);

  uint32_t inst = cpu.fetch();
  cpu.execute(inst);

  // 检查x1寄存器是否被正确设置为1
  TestFramework::assertEqual("Test ADDI Instruction Execution",
                             std::to_string(cpu.getReg(1)), std::to_string(1));

  std::cout << "Passed testExecuteAddiInstruction.\n";
}

void testExecuteAddInstruction() {
  std::cout << "Starting testExecuteAddInstruction...\n";

  // 示例指令代码：add x4, x0, x2
  std::vector<uint8_t> code = {0x33, 0x02, 0x20, 0x00};
  Cpu cpu(code);

  // 设置寄存器x2的初始值
  cpu.setReg(2, 20);  // x2 = 20

  uint32_t inst = cpu.fetch();
  std::cout << "Fetched instruction: 0x" << std::hex << inst << std::endl;
  cpu.execute(inst);

  // 检查x4寄存器是否被正确设置为x0 + x2，即20
  TestFramework::assertEqual("Test ADD Instruction Execution",
                             std::to_string(cpu.getReg(4)), std::to_string(20));

  std::cout << "Passed testExecuteAddInstruction.\n";
}

int main() {
  std::cout << "Starting CPU tests...\n";

  testCpuInitialization();
  testCpuRegisterModification();
  testMemoryModification();
  testFetchInstruction();
  testExecuteAddiInstruction();
  testExecuteAddInstruction();

  std::cout << "All tests completed successfully.\n";
  return 0;
}
