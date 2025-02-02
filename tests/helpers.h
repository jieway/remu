#ifndef HELPERS_H
#define HELPERS_H

#include <cstdint>
#include <expected>
#include <string>
#include <vector>

/**
 * @brief 🚀 生成二进制机器码的辅助函数
 *
 * 📦 该函数将汇编代码编译为可执行的二进制格式，并返回二进制数据或错误信息
 *
 * @param asm_code 📝 需要编译的汇编代码字符串
 * @param base_filename 🏷️ 用于生成临时文件的基础文件名（不含扩展名）
 * @param log_prefix 📌 日志前缀信息（默认值："Loaded binary file size: "）
 *
 * @return std::expected<std::vector<uint8_t>, std::string> 🎯 返回结果包含：
 *         - 成功：包含二进制机器码的字节向量
 *         - 失败：包含错误描述信息的字符串
 *
 * @note 💡 使用 std::expected 需要 C++23 或更高标准
 * @warning ❌ 可能发生的错误情况：
 *          - 汇编器路径配置错误
 *          - 临时文件创建失败
 *          - 汇编编译过程出错
 */
[[nodiscard]] std::expected<std::vector<uint8_t>, std::string> generate_binary(
    const std::string& asm_code, const std::string& base_filename,
    const std::string& log_prefix = "Loaded binary file size: ");

#endif  // HELPERS_H
