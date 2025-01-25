#include <array>
#include <cstdint>
#include <vector>

// 🖥️ CPU模拟器结构体：模拟RISC架构处理器核心组件
struct Cpu {
  std::array<uint64_t, 32> regs{};  // 🧠 32个通用寄存器，初始化为0（x0-x31）
  uint64_t pc = 0;  // 📍 程序计数器：跟踪当前执行指令地址
  std::vector<uint8_t> mem;  // 💾 模拟内存：存储程序指令和数据

  // 🏗️ 构造函数：用程序代码初始化CPU
  // 📥 参数：code - 要加载到内存中的机器码
  Cpu(const std::vector<uint8_t>& code) : mem(code) {
    // 🚀 初始化堆栈指针（通常x2寄存器在RISC-V中作为栈指针）
    // 📌 栈指针指向内存末尾（假设栈向下增长）
    regs[2] = mem.size() - 1;
  }
};

int main() {
  // 📦 创建示例代码（1KB空数据，用于演示内存初始化）
  std::vector<uint8_t> code(1024);

  // 🔌 创建CPU实例并加载代码
  Cpu cpu(code);

  // 💡 提示：在实际模拟器中，这里会添加指令解码和执行循环
  // 🎮 例如：while (running) { fetch(); decode(); execute(); }
}