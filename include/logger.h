#ifndef LOGGER_H
#define LOGGER_H

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <source_location>
#include <sstream>
#include <string_view>

// 🎯 定义日志级别枚举，代表不同严重程度的日志类型
enum class LogLevel {
  Debug,  // 🔍 调试信息，仅用于调试过程中输出详细信息
  Info,   // ℹ️ 一般信息，用于记录正常的系统运行状况
  Warn,  // ⚠️ 警告信息，提示可能存在的问题，但不影响程序运行
  Error,  // ❌ 错误信息，记录执行过程中发生的错误
  Fatal   // 💀 致命错误，表示严重错误，需要立即处理
};

// 🎨 定义 ANSI 颜色代码，用于在终端中为日志消息添加颜色
class Colors {
 public:
  // 重置颜色
  static constexpr const char* RESET = "\033[0m";
  // 灰色：用于调试信息
  static constexpr const char* GRAY = "\033[90m";
  // 绿色：用于一般信息
  static constexpr const char* GREEN = "\033[32m";
  // 黄色：用于警告
  static constexpr const char* YELLOW = "\033[33m";
  // 红色：用于错误信息
  static constexpr const char* RED = "\033[31m";
  // 品红：用于致命错误
  static constexpr const char* MAGENTA = "\033[35m";
};

// 📝 日志记录器类，用于格式化并输出带有详细上下文信息的日志
class Logger {
 public:
  // 🏭 单例模式：获取 Logger 实例，确保全局只有一个日志记录器
  static Logger& instance() {
    static Logger logger;
    return logger;
  }

  // 🚫 删除拷贝构造与赋值操作，防止无意中的复制
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  // 📝 模板函数：记录调试级别日志，自动捕获调用位置
  template <typename... Args>
  static void debug(Args&&... args) {
    instance().log(LogLevel::Debug, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  // 📝 模板函数：记录信息级别日志，自动捕获调用位置
  template <typename... Args>
  static void info(Args&&... args) {
    instance().log(LogLevel::Info, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  // 📝 模板函数：记录警告级别日志，自动捕获调用位置
  template <typename... Args>
  static void warn(Args&&... args) {
    instance().log(LogLevel::Warn, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  // 📝 模板函数：记录错误级别日志，自动捕获调用位置
  template <typename... Args>
  static void error(Args&&... args) {
    instance().log(LogLevel::Error, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  // 📝 模板函数：记录致命错误日志，自动捕获调用位置
  template <typename... Args>
  static void fatal(Args&&... args) {
    instance().log(LogLevel::Fatal, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  // 🎚️ 设置最低日志级别，只有级别不低于该设置的日志才会被输出
  void set_min_level(LogLevel level) { min_level_ = level; }

 private:
  // 🏗️ 私有构造函数，保证 Logger 只能通过 instance() 方法获取（单例模式）
  Logger() = default;

  // 📝 核心日志记录模板函数
  // 参数：
  //   level    - 当前日志的级别
  //   location - 调用日志函数的代码位置（文件名、行号）
  //   args     - 日志消息主体，支持多个参数拼接
  template <typename... Args>
  void log(LogLevel level, const std::source_location& location,
           Args&&... args) {
    // 如果日志的级别低于当前设置的最低输出级别，则忽略该日志
    if (level < min_level_) return;

    // 🔒 加锁保证多线程环境下的日志输出不会交叉混乱
    std::lock_guard<std::mutex> lock(mutex_);

    // ⏰ 获取当前系统时间
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    // 📊 构建日志消息
    std::stringstream ss;
    ss << level_colors[static_cast<size_t>(level)]  // 设置颜色
       << "["
       << std::put_time(std::localtime(&timestamp),
                        "%Y-%m-%d %H:%M:%S")
       << "] "                                      // 添加时间戳
       << level_emojis[static_cast<size_t>(level)]  // 添加对应的 emoji
       << " "
       << level_strings[static_cast<size_t>(level)]  // 添加日志级别字符串
       << " " << "[" << location.file_name() << ":" << location.line()
       << "] ";  // 添加文件和行号信息

    // 利用折叠表达式将所有日志参数拼接到字符串流中
    (ss << ... << std::forward<Args>(args)) << Colors::RESET << std::endl;

    // 🖨️ 输出最终构建的日志消息到标准输出
    std::cout << ss.str();
  }

  std::mutex mutex_;  // 🔒 互斥锁，确保线程安全的日志输出
  LogLevel min_level_ = LogLevel::Debug;  // 🎚️ 日志输出的最低级别，默认为 Debug

  // 📝 对应各日志级别的字符串表示
  static constexpr std::array<const char*, 5> level_strings = {
      "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

  // 😀 对应各日志级别的 emoji 表情
  static constexpr std::array<const char*, 5> level_emojis = {"🔍", "ℹ️ ", "⚠️ ",
                                                              "❌", "💀"};

  // 🎨 对应各日志级别的终端颜色
  static constexpr std::array<const char*, 5> level_colors = {
      Colors::GRAY,    // Debug - 灰色
      Colors::GREEN,   // Info - 绿色
      Colors::YELLOW,  // Warn - 黄色
      Colors::RED,     // Error - 红色
      Colors::MAGENTA  // Fatal - 品红色
  };
};

#endif  // LOGGER_H