#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include "cup.h"
#include "log.h"

#define SQUARE_DO_WHILE(x) do { printf("Calculating square... "); x * x; } while(0);

#define SQUARE_BRACES(x) { printf("Calculating square... "); x * x; }

void some_function() {
  std::cout << "sss" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    LOG(ERROR, "Usage:\n- ./program_name <filename>");
    return 0;
  }

  std::ifstream file(argv[1], std::ios::binary);
  if (!file) {
    LOG(ERROR, "Cannot open file: ", argv[1]);
    return 1;
  }

  std::vector<uint8_t> code(std::istreambuf_iterator<char>(file), {});
  crvemu::Cpu cpu(code); // 假设Cpu类的构造函数接受指令代码的vector

  while (true) {
    auto inst = cpu.fetch();
    if (!inst.has_value()) {
      LOG(INFO, "End of program reached.");
      break;
    }
    auto new_pc = cpu.execute(inst.value());
    if (!new_pc.has_value()) {
      LOG(ERROR, "Execution of instruction failed.");
      break;
    }

    cpu.pc = new_pc.value();
  }
  // 使用cpu对象进行操作
  cpu.dump_registers(); // 打印寄存器状态
  cpu.dump_pc();

  return 0;
}