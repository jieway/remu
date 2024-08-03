#include "cpu.h"
#include <vector>

int main() {
  std::vector<uint8_t> code = {1, 2, 3, 4, 5, 6, 7, 8};
  Cpu cpu(code);

  cpu.printRegs();
  cpu.printMem();
  std::cout << "Initialization successful!" << std::endl;
  return 0;
}
