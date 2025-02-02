#ifndef HELPERS_H
#define HELPERS_H

#include <chrono>
#include <cstdint>
#include <expected>
#include <format>
#include <functional>
#include <iomanip>
#include <iostream>
#include <source_location>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "logger.h"

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

/**
 * @brief 📊 测试统计数据结构
 *
 * 🧮 用于记录测试框架运行时的各项指标数据，
 * 包含总测试数、通过数、失败数和总耗时
 */
struct TestStats {
  int total = 0;           ///< 🧺 总测试用例数
  int passed = 0;          ///< ✅ 通过用例数
  int failed = 0;          ///< ❌ 失败用例数
  double totalTime = 0.0;  ///< ⏱️ 测试执行总时长（单位：秒）
};

/**
 * ETest 测试框架
 *
 * 使用单例模式封装所有测试用例，并提供自动注册、计时统计、
 * 彩色输出和改进断言功能。使用该框架只需要借助 TEST 宏定义测试用例，
 * 框架会自动在程序启动时通过全局对象进行注册。
 */
class ETest {
 public:
  // 获取单例实例（线程安全、懒汉式初始化）
  static inline ETest& GetInstance() {
    static ETest instance;
    return instance;
  }

  // 测试函数类型，支持 lambda 表达式、函数对象等；现在使用 std::expected
  // 指示测试结果
  using TestFunction = std::function<std::expected<void, std::string>()>;

  // 注册测试用例，采用值传递并移动，以保证参数明确且高效
  void AddTest(std::string name, TestFunction test_func) {
    tests_.emplace_back(std::move(name), std::move(test_func));
  }

  // 测试执行器，运行所有注册的测试并统计计时、测试通过与失败情况
  void RunAllTests() noexcept {
    TestStats stats;
    LOG_INFO("\033[1;36mStarting tests...\033[0m");

    for (const auto& test : tests_) {
      stats.total++;
      auto start = std::chrono::high_resolution_clock::now();

      LOG_INFO("Running: " + test.name);
      auto result = test.func();
      if (result.has_value()) {
        stats.passed++;
        LOG_INFO("\033[1;32mPASSED\033[0m: " + test.name);
      } else {
        stats.failed++;
        LOG_ERROR("\033[1;31mFAILED\033[0m: " + test.name +
                  "\n  Error: " + result.error());
      }

      auto end = std::chrono::high_resolution_clock::now();
      stats.totalTime += std::chrono::duration<double>(end - start).count();
    }

    PrintTestSummary(stats);
  }

 private:
  // 定义测试用例结构体，用来存储测试描述和测试函数
  struct TestCase {
    std::string name;   // 测试用例名称
    TestFunction func;  // 测试函数

    // 构造函数中采用移动语义初始化
    TestCase(std::string n, TestFunction f)
        : name(std::move(n)), func(std::move(f)) {}
  };

  // 📦 存储所有注册的测试用例的容器
  // 🚨 注意：使用 vector 保证测试执行顺序与注册顺序一致
  std::vector<TestCase> tests_;

  /**
   * @brief 🏗️ 单例构造函数
   *
   * 🔧 使用 reserve 预分配空间优化性能，避免多次内存分配
   * 📌 初始容量设置为 100 个测试用例的存储空间
   */
  ETest() { tests_.reserve(100); }

  // 🚫 显式禁止拷贝构造和赋值操作
  // ⚠️ 保证单例模式的唯一性，防止意外复制
  ETest(const ETest&) = delete;
  ETest& operator=(const ETest&) = delete;

  // 输出美观的测试统计报告
  void PrintTestSummary(const TestStats& stats) const {
    LOG_INFO("=========================");
    LOG_INFO("Test Summary:");
    LOG_INFO("Total: " + std::to_string(stats.total) + " tests");
    LOG_INFO("\033[1;32mPassed: " + std::to_string(stats.passed) + "\033[0m");

    if (stats.failed == 0) {
      // 如果失败数为 0，则使用绿色输出
      LOG_INFO("\033[1;32mFailed: " + std::to_string(stats.failed) + "\033[0m");
    } else {
      // 如果失败数大于 0，则使用红色输出
      LOG_INFO("\033[1;31mFailed: " + std::to_string(stats.failed) + "\033[0m");
    }

    LOG_INFO(std::format("Time: {:.3f} s", stats.totalTime));
    LOG_INFO("=========================");
  }
};

/**
 * @brief 🛡️ 增强型断言宏
 *
 * 💥 当条件不满足时：
 * 1. 记录错误日志
 * 2. 返回包含错误信息的 unexpected 对象
 *
 * @param condition 要验证的条件表达式
 *
 * @example
 * ASSERT(1 + 1 == 2);
 * ASSERT(!vector.empty());
 */
#define ASSERT(condition)                                                \
  if (!(condition)) {                                                    \
    const std::string message =                                          \
        std::format("Assertion failed at {}:{}: {}",                     \
                    std::source_location::current().file_name(),         \
                    std::source_location::current().line(), #condition); \
    LOG_ERROR(message);                                                  \
    return std::unexpected(message);                                     \
  }

/**
 * @brief ⚖️ 相等断言宏
 *
 * 🔍 深度比较两个值的相等性：
 * 1. 支持所有重载了 == 操作符的类型
 * 2. 自动推导类型，避免隐式转换
 * 3. 输出易读的对比信息
 *
 * @param expected 期望值（左侧）
 * @param actual 实际值（右侧）
 *
 * @example
 * ASSERT_EQ(42, calculateAnswer());
 * ASSERT_EQ("hello", greeting);
 */
#define ASSERT_EQ(expected, actual)                          \
  do {                                                       \
    const auto& exp = (expected);                            \
    const auto& act = (actual);                              \
    if (exp != act) {                                        \
      const std::string message =                            \
          std::format("Expected: {}\nActual: {}", exp, act); \
      LOG_ERROR(message);                                    \
      return std::unexpected(message);                       \
    }                                                        \
  } while (0)

// TEST 宏帮助自动注册测试用例，利用静态对象的构造函数在程序启动前完成注册
#define TEST(name)                                                         \
  std::expected<void, std::string> test_##name();                          \
  struct Register##name {                                                  \
    Register##name() { ETest::GetInstance().AddTest(#name, test_##name); } \
  } register##name##Instance;                                              \
  std::expected<void, std::string> test_##name()

#endif  // HELPERS_H