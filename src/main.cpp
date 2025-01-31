#include <fstream>
#include <iostream>
#include <vector>

#include "cpu.h"
#include "logger.h"

/**
 * @函数名称: main
 * @功能描述: 🚀 RISC-V模拟器主程序入口
 * @参数说明: argc - 命令行参数数量, argv - 命令行参数数组
 * @返回值: int - 程序退出码(0表示成功,1表示错误)
 */
int main(int argc, char* argv[]) {
  // 🔍 检查命令行参数
  if (argc != 2) {
    Logger::error("Usage: ", argv[0], " <binary-file>");
    return 1;
  }

  // 📂 以二进制模式打开RISC-V程序文件
  // 💡 ios::ate表示文件指针初始位置在文件末尾,用于获取文件大小
  std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
  if (!file) {
    Logger::error("Unable to open binary file: ", argv[1]);
    return 1;
  }

  // 📏 获取文件大小并进行基本检查
  std::streamsize size = file.tellg();
  if (size < 4) {
    Logger::error("Binary file is too small (minimum 4 bytes required)");
    return 1;
  }

  // 🔢 计算4字节对齐的内存大小
  // 💡 使用位运算确保大小是4的倍数
  std::streamsize aligned_size = (size + 3) & ~3;

  // 📥 读取程序到内存
  file.seekg(0, std::ios::beg);  // 将文件指针重置到开始位置
  std::vector<uint8_t> program(aligned_size, 0);  // 初始化内存缓冲区为0
  file.read(reinterpret_cast<char*>(program.data()), size);

  // 🖥️ 创建并初始化CPU实例
  Cpu cpu(program);
  Logger::info("RISC-V Simulator Started (Memory: ", size, " bytes)");

  // ⚡ 主执行循环：获取-执行-调试
  while (cpu.pc < cpu.mem.size()) {
    // 🔒 安全检查：确保PC不会越界
    if (cpu.pc + 3 >= cpu.mem.size()) {
      Logger::warn("PC out of bounds at 0x", std::hex, cpu.pc);
      break;
    }

    // 📡 取指：从内存获取当前指令
    uint32_t instr = cpu.fetch();

    // 🎯 执行：处理当前指令
    cpu.execute(instr);

    // 🔍 调试：输出CPU状态信息
    cpu.debug();
  }

  // 🏁 模拟器执行完成
  Logger::info("\nSimulation Completed");
  return 0;
}