#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include "cup.h"
#include "exception.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage:\n"
                  << "- ./program_name <filename>\n";
        return 0;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << argv[1] << std::endl;
        return 1;
    }

    std::vector<uint8_t> code(std::istreambuf_iterator<char>(file), {});
    cemu::Cpu cpu(code); // 假设Cpu类的构造函数接受指令代码的vector


    try {
        while (true) {
            uint32_t inst = cpu.fetch();
            auto new_pc = cpu.execute(inst);
            if (new_pc.has_value()) {
                cpu.pc = new_pc.value();
            } else {
                break;
            }
        }
    } catch (const cemu::Exception& e) {
        std::cerr << "Exception main: " << e << std::endl;
    }

    // 使用cpu对象进行操作
    cpu.dump_registers(); // 打印寄存器状态
    cpu.dump_pc();

    return 0;
}