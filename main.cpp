#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip> // 用于格式化输出
#include <stdexcept> // 用于异常处理

// 定义DRAM_SIZE为128MB
const uint64_t DRAM_SIZE = 1024 * 1024 * 128;

class Cpu {
public:
    // RISC-V 有 32 个寄存器
    std::array<uint64_t, 32> regs;

    // PC 寄存器包含下一条指令的内存地址
    uint64_t pc;

    // 内存，一个字节数组。在真实的 CPU 中没有内存，这里仅作模拟。
    std::vector<uint8_t> dram;

    // RISC-V 寄存器名称
    const std::array<std::string, 32> RVABI = {
        "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
        "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
        "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
        "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",
    };

    // 构造函数
    Cpu(const std::vector<uint8_t>& code) : pc(0), dram(code) {
        regs.fill(0); // 初始化寄存器为0
        regs[2] = DRAM_SIZE - 1; // 设置堆栈指针寄存器的初始值
    }

    // Fetch函数用于读取当前pc指向的指令
    uint32_t fetch() {
        size_t index = static_cast<size_t>(pc); // 确保pc值在转换时不会丢失信息
        uint32_t inst = static_cast<uint32_t>(dram[index])
                      | (static_cast<uint32_t>(dram[index + 1]) << 8)
                      | (static_cast<uint32_t>(dram[index + 2]) << 16)
                      | (static_cast<uint32_t>(dram[index + 3]) << 24);
        return inst;
    }

    void execute(uint32_t inst) {
        uint32_t opcode = inst & 0x7f;
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        uint32_t rs2 = (inst >> 20) & 0x1f;
        uint32_t funct3 = (inst >> 12) & 0x7;
        uint32_t funct7 = (inst >> 25) & 0x7f;

        // x0 is hardwired zero
        regs[0] = 0;

        // execute stage
        switch (opcode) {
            case 0x13: { // addi
                int64_t imm = static_cast<int32_t>(inst & 0xfff00000) >> 20;
                regs[rd] = regs[rs1] + imm;
                break;
            }
            case 0x33: { // add
                regs[rd] = regs[rs1] + regs[rs2];
                break;
            }
            default:
                std::cerr << "Invalid opcode: " << std::hex << opcode << std::endl;
            break;
        }
    }

    void dump_registers() {
        std::cout << std::setw(80) << std::setfill('-') << "" << std::endl; // 打印分隔线
        std::cout << std::setfill(' '); // 重置填充字符
        for (size_t i = 0; i < 32; i += 4) {
            std::cout << std::setw(4) << "x" << i << "(" << RVABI[i] << ") = " << std::hex << std::setw(16) << std::setfill('0') << regs[i] << " "
                      << std::setw(4) << "x" << i + 1 << "(" << RVABI[i + 1] << ") = " << std::setw(16) << regs[i + 1] << " "
                      << std::setw(4) << "x" << i + 2 << "(" << RVABI[i + 2] << ") = " << std::setw(16) << regs[i + 2] << " "
                      << std::setw(4) << "x" << i + 3 << "(" << RVABI[i + 3] << ") = " << std::setw(16) << regs[i + 3] << std::endl;
        }
    }
};

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
    Cpu cpu(code); // 假设Cpu类的构造函数接受指令代码的vector

    while (cpu.pc < cpu.dram.size() * sizeof(cpu.dram[0])) {
        uint32_t inst = cpu.fetch();
        cpu.execute(inst);
        cpu.pc += 4; // 假设指令长度固定为4字节
    }

    // 使用cpu对象进行操作
    cpu.dump_registers(); // 打印寄存器状态
    return 0;
}