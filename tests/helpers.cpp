#include "helpers.h"

#include <cstdlib>
#include <fstream>

#include "logger.h"

/**
 * 📝 生成 RISC-V 二进制文件（完整工作流程）
 * @param asm_code 汇编代码字符串
 * @param base_filename 临时文件基础名称
 * @param log_prefix 日志前缀标识
 * @return 包含二进制数据的vector，或错误信息
 *
 * 🚨 注意：本函数会创建临时文件并自动清理
 * ✅ 安全特性：使用 [[nodiscard]] 确保返回值必须处理
 */
[[nodiscard]] std::expected<std::vector<uint8_t>, std::string> generate_binary(
    const std::string& asm_code, const std::string& base_filename,
    const std::string& log_prefix) {
  std::string asm_filename = base_filename + ".s";
  std::string obj_filename = base_filename + ".o";
  std::string bin_filename = base_filename + ".bin";

  // 🔧 步骤1：创建临时汇编文件
  // 生成三个临时文件名（.s汇编文件 / .o目标文件 / .bin二进制文件）
  std::ofstream asm_file(asm_filename);
  if (!asm_file) {
    return std::unexpected("Failed to create assembly file " + asm_filename);
  }
  asm_file << asm_code;
  asm_file.close();

  // 🛠️ 步骤2：调用 RISC-V 汇编器
  // 使用参数说明：
  // - -march=rv32i: 指定 32 位基础指令集
  // - -mabi=ilp32: 指定应用程序二进制接口
  std::string assembler_cmd =
      "riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 -o " + obj_filename +
      " " + asm_filename;
  int ret = std::system(assembler_cmd.c_str());
  if (ret != 0) {
    std::system(("rm -f " + asm_filename).c_str());
    return std::unexpected("Assembly command failed!");
  }

  // 🔄 步骤3：二进制转换（使用 objcopy）
  // 功能：从目标文件中提取纯二进制数据
  // 注意：此处使用 -O binary 参数生成原始二进制格式
  std::string objcopy_cmd = "riscv64-unknown-elf-objcopy -O binary " +
                            obj_filename + " " + bin_filename;
  ret = std::system(objcopy_cmd.c_str());
  if (ret != 0) {
    std::system(("rm -f " + asm_filename + " " + obj_filename).c_str());
    return std::unexpected("objcopy command failed!");
  }

  // 📦 步骤4：读取二进制文件
  // 安全措施：
  // 1. 使用二进制模式打开（std::ios::binary）
  // 2. 立即获取文件大小（std::ios::ate）
  // 3. 验证最小有效大小（至少4字节）
  std::ifstream bin_file(bin_filename, std::ios::binary | std::ios::ate);
  if (!bin_file) {
    std::system(
        ("rm -f " + asm_filename + " " + obj_filename + " " + bin_filename)
            .c_str());
    return std::unexpected("Failed to open binary file " + bin_filename);
  }
  std::streamsize size = bin_file.tellg();
  if (size < 4) {
    std::system(
        ("rm -f " + asm_filename + " " + obj_filename + " " + bin_filename)
            .c_str());
    return std::unexpected("Generated binary file is too small");
  }
  bin_file.seekg(0, std::ios::beg);
  std::vector<uint8_t> code(static_cast<size_t>(size));
  if (!bin_file.read(reinterpret_cast<char*>(code.data()), size)) {
    std::system(
        ("rm -f " + asm_filename + " " + obj_filename + " " + bin_filename)
            .c_str());
    return std::unexpected("Failed to read binary file " + bin_filename);
  }
  bin_file.close();

  // 📊 日志记录：使用 LOG_INFO 宏记录生成的二进制文件大小
  LOG_INFO(log_prefix, static_cast<uint64_t>(size), " bytes");

  // 🧹 步骤5：资源清理
  // 安全特性：无论成功与否都会执行清理
  // 清理内容：所有中间临时文件（.s/.o/.bin）
  std::string cleanup_cmd =
      "rm -f " + asm_filename + " " + obj_filename + " " + bin_filename;
  std::system(cleanup_cmd.c_str());

  return code;
}