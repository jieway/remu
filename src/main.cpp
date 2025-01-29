#include <iostream>

#include "cpu.h"

int main() {
  // 📦 初始化模拟器
  std::vector<uint8_t> program(1024);  // 初始化1KB的程序内存
  Cpu cpu(program);

  // 🚀 启动模拟器
  std::cout << "RISC-V Simulator Starting..." << std::endl;

  // TODO: 在这里添加实际的程序加载和执行逻辑

  return 0;
}