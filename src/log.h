//
// Created by Jie Wei on 2024/2/12.
//

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string_view>

constexpr std::string_view RED = "\033[0;31m";
constexpr std::string_view GREEN = "\033[0;32m";
constexpr std::string_view YELLOW = "\033[0;33m";
constexpr std::string_view BLUE = "\033[0;34m";
constexpr std::string_view DIM = "\033[0;37m";
constexpr std::string_view NC = "\033[0m";  // No color

// Log 级别
enum LogLevel {
  DEBUG,
  INFO,
  WARNING,
  ERROR
};

// 接受任何可以通过 << 输出的类型
template <typename T>
concept Printable = requires(std::ostream& os, T s) {
  os << s;
};

// 是否启用 debug panic 输出
constexpr bool ENABLE_DEBUG_PANIC = true;

// 打印日志函数，接受可打印类型的参数（Printable... Args）
template <Printable... Args>
void print_log(std::ostream& os, LogLevel level, Args&&... s) {
  // 定义日志级别字符串数组
  const char* levelStrings[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
  // 定义颜色字符串视图数组，对应不同的日志级别
  const std::string_view colors[] = {BLUE, GREEN, YELLOW, RED};

  // 输出带颜色的日志级别标识
  os << colors[level] << "[" << levelStrings[level] << "] ";
  // 使用折叠表达式将所有参数连接到输出流中
  (os << ... << s);
  // 输出颜色结束标识（假设NC被定义为颜色结束符号）
  os << NC << '\n';

  // 如果日志级别为 ERROR 并且启用了 debug panic 输出，则调用 abort
  if (level == ERROR && ENABLE_DEBUG_PANIC) {
    std::cerr << "Error occurred. Aborting program." << std::endl;
    std::abort();
  }
}

#define LOG(level, ...)                                             \
  do {                                                              \
    print_log(std::cout, level, "In function ", __FUNCTION__, " (", \
    __FILE__, ':', __LINE__, "): ", __VA_ARGS__);                   \
  } while (0)

#endif //LOG_H
