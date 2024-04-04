#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include "cup.h"
#include "log.h"
#include "exception.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    LOG(cemu::ERROR, "Usage:\n- ./program_name <filename>");
    return 0;
  }

  std::ifstream file(argv[1], std::ios::binary);
  if (!file) {
    LOG(cemu::ERROR, "Cannot open file: ", argv[1]);
    return 1;
  }

  std::vector<uint8_t> code(std::istreambuf_iterator<char>(file), {});
  cemu::Cpu cpu(code); // 假设Cpu类的构造函数接受指令代码的vector

  while (true) {
    try {
      auto inst = cpu.fetch();
      if (!inst.has_value()) {
        LOG(cemu::INFO, "End of program reached.");
        break;
      }
      auto new_pc = cpu.execute(inst.value());
      cpu.pc = new_pc.value();
    } catch (const cemu::Exception& e) {
      cpu.handle_exception(e);
      if (e.isFatal()) {
        LOG(cemu::INFO, "Fatal error: ", e.what());
        break;
      }
    }
    auto interrupt = cpu.check_pending_interrupt();
    if (interrupt.has_value()) {
      cpu.handle_interrupt(interrupt.value());
    }
  }

  cpu.dump_registers(); // 打印寄存器状态
  cpu.dump_pc();        // 打印PC寄存器状态
  return 0;
}