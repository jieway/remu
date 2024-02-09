#include "test_util.h"

namespace crvemu {
    void generate_rv_assembly(const std::string& c_src) {
        std::string command = "riscv64-unknown-elf-gcc -S" + c_src + " -o ";
        int result = std::system(command.c_str());

        if (result != 0) {
            throw std::runtime_error("Failed to generate RV assembly. Command: " + command);
        }
    }

    void generate_rv_obj(const std::string& assembly) {
        // 使用C++的字符串处理能力来获取不含扩展名的文件名
        size_t dotPos = assembly.find_last_of(".");
        std::string baseName = (dotPos == std::string::npos) ? assembly : assembly.substr(0, dotPos);

        // 构建clang命令行字符串
        std::string command = "riscv64-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib -o " + baseName + " " + assembly;

        // 执行命令
        int result = std::system(command.c_str());

        // 检查命令执行结果
        if (result != 0) {
            std::cerr << "Failed to generate RV object from assembly: " << assembly << std::endl;
        }
    }

    void generate_rv_binary(const std::string& obj) {
        // 构建llvm-objcopy命令行字符串
        std::string command = "riscv64-unknown-elf-objcopy -O binary " + obj + " " + obj + ".bin";

        // 执行命令
        int result = std::system(command.c_str());

        // 检查命令执行结果
        if (result != 0) {
            std::cerr << "Failed to generate RV binary from object: " << obj << std::endl;
        }
    }

    Cpu rv_helper(const std::string& code, const std::string& testname, size_t n_clock) {
        std::string filename = testname + ".s";
        // 创建并写入汇编文件
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create assembly file.");
        }
        file << code;
        file.close();

        // 生成目标文件和二进制文件
        generate_rv_obj(filename.c_str());
        generate_rv_binary(testname.c_str());

        // 读取二进制文件内容
        std::string binFilename = testname + ".bin";
        std::ifstream file_bin(binFilename, std::ios::binary);
        if (!file_bin.is_open()) {
            throw std::runtime_error("Failed to open binary file.");
        }
        std::vector<uint8_t> binaryCode((std::istreambuf_iterator<char>(file_bin)), std::istreambuf_iterator<char>());

        // 初始化CPU并执行指令
        Cpu cpu(binaryCode);
        for (size_t i = 0; i < n_clock; ++i) {
            try {
                auto inst = cpu.fetch();
              if (inst.has_value()) {
                auto new_pc = cpu.execute(inst.value());
                if (new_pc.has_value()) {
                  cpu.pc = new_pc.value();
                } else {
                  break;
                }
              } else {
                break;
              }
            } catch (const std::exception& e) {
                std::cerr << "CPU execution error: " << e.what() << std::endl;
                break;
            }
        }
        return cpu;
    }
}