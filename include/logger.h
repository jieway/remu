#ifndef LOGGER_H
#define LOGGER_H

#include <array>
#include <chrono>
#include <iostream>
#include <mutex>
#include <source_location>
#include <sstream>
#include <string_view>

// 🎯 定义日志级别
enum class LogLevel {
  Debug,  // 🔍 调试信息
  Info,   // ℹ️ 一般信息
  Warn,   // ⚠️ 警告信息
  Error,  // ❌ 错误信息
  Fatal   // 💀 致命错误
};

// 🎨 ANSI 颜色代码
class Colors {
 public:
  static constexpr const char* RESET = "\033[0m";
  static constexpr const char* GRAY = "\033[90m";
  static constexpr const char* GREEN = "\033[32m";
  static constexpr const char* YELLOW = "\033[33m";
  static constexpr const char* RED = "\033[31m";
  static constexpr const char* MAGENTA = "\033[35m";
};

// 📝 日志记录器类
class Logger {
 public:
  // 🏭 获取单例实例
  static Logger& instance() {
    static Logger logger;
    return logger;
  }

  // 🚫 删除拷贝构造和赋值操作
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  // 📝 创建日志记录对象的工厂方法
  template <typename... Args>
  static void debug(Args&&... args) {
    instance().log(LogLevel::Debug, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void info(Args&&... args) {
    instance().log(LogLevel::Info, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void warn(Args&&... args) {
    instance().log(LogLevel::Warn, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void error(Args&&... args) {
    instance().log(LogLevel::Error, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void fatal(Args&&... args) {
    instance().log(LogLevel::Fatal, std::source_location::current(),
                   std::forward<Args>(args)...);
  }

  // 🎚️ 设置最小日志级别
  void set_min_level(LogLevel level) { min_level_ = level; }

 private:
  // 🏗️ 私有构造函数（单例模式）
  Logger() = default;

  // 📝 日志记录主方法
  template <typename... Args>
  void log(LogLevel level, const std::source_location& location,
           Args&&... args) {
    if (level < min_level_) return;

    std::lock_guard<std::mutex> lock(mutex_);

    // ⏰ 获取当前时间
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    // 📊 构建日志消息
    std::stringstream ss;
    ss << level_colors[static_cast<size_t>(level)] << "["
       << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S") << "] "
       << level_emojis[static_cast<size_t>(level)] << " "
       << level_strings[static_cast<size_t>(level)] << " " << "["
       << location.file_name() << ":" << location.line() << "] ";

    (ss << ... << std::forward<Args>(args)) << Colors::RESET << std::endl;

    // 🖨️ 输出日志
    std::cout << ss.str();
  }

  std::mutex mutex_;                      // 🔒 线程安全锁
  LogLevel min_level_ = LogLevel::Debug;  // 🎚️ 最小日志级别

  // 📝 日志级别对应的字符串
  static constexpr std::array<const char*, 5> level_strings = {
      "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

  // 😀 日志级别对应的表情符号
  static constexpr std::array<const char*, 5> level_emojis = {"🔍", "ℹ️ ", "⚠️ ",
                                                              "❌", "💀"};

  // 🎨 日志级别对应的颜色
  static constexpr std::array<const char*, 5> level_colors = {
      Colors::GRAY,    // Debug
      Colors::GREEN,   // Info
      Colors::YELLOW,  // Warn
      Colors::RED,     // Error
      Colors::MAGENTA  // Fatal
  };
};

#endif  // LOGGER_H