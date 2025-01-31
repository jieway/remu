#include <fstream>
#include <iostream>
#include <vector>

#include "cpu.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <binary-file>" << std::endl;
    return 1;
  }

  // 📥 加载RISC-V二进制程序
  std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
  if (!file) {
    std::cerr << "Error: Unable to open binary file: " << argv[1] << std::endl;
    return 1;
  }

  // 📦 将文件内容读入内存
  std::streamsize size = file.tellg();
  if (size < 4) {
    std::cerr << "Error: Binary file is too small (minimum 4 bytes required)"
              << std::endl;
    return 1;
  }

  // 计算对齐后的内存大小（向上取整到4字节的倍数）
  std::streamsize aligned_size = (size + 3) & ~3;

  // 📦 将文件内容读入内存（使用对齐后的大小）
  file.seekg(0, std::ios::beg);
  std::vector<uint8_t> program(aligned_size, 0);  // 初始化为0
  file.read(reinterpret_cast<char*>(program.data()), size);

  // 🖥️ 初始化CPU
  Cpu cpu(program);
  std::cout << "🚀 RISC-V Simulator Started (Memory: " << size << " bytes)"
            << std::endl;

  // ⚙️ 执行循环
  while (cpu.pc < cpu.mem.size()) {
    // 📡 取指
    if (cpu.pc + 3 >= cpu.mem.size()) {
      std::cerr << "Warning: PC out of bounds at 0x" << std::hex << cpu.pc
                << std::endl;
      break;
    }
    uint32_t instr = cpu.fetch();

    // ⚡ 执行
    cpu.execute(instr);

    // 🐛 调试输出（可选）
    cpu.debug();
  }

  std::cout << "\nSimulation Completed" << std::endl;
  return 0;
}