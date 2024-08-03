#ifndef TEST_FRAMEWORK_HPP
#define TEST_FRAMEWORK_HPP

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

// ANSI color codes for pretty output
constexpr std::string_view RESET = "\033[0m";
constexpr std::string_view RED = "\033[1;31m";
constexpr std::string_view GREEN = "\033[1;32m";
constexpr std::string_view YELLOW = "\033[1;33m";
constexpr std::string_view CYAN = "\033[1;36m";
constexpr std::string_view BOLD = "\033[1m";

class TestFramework {
public:
  static void printResult(bool success, std::string_view testName) {
    std::cout << (success ? GREEN : RED) << BOLD
              << (success ? "ok  " : "FAILED") << RESET << " - " << testName
              << "\n";
  }

  static void assertEqual(std::string_view testName, std::string_view expected,
                          std::string_view actual) {
    bool success = expected == actual;
    printResult(success, testName);
    if (!success) {
      std::cerr << YELLOW << "Expected: " << RESET << expected << "\n"
                << CYAN << "Actual:   " << RESET << actual << "\n";
    }
  }

  // TokenType 和 tokenTypeToStringFunc 需要在使用此框架的项目中自行定义
  template <typename TokenType>
  static void assertEqual(std::string_view testName, TokenType expected,
                          TokenType actual,
                          std::string (*tokenTypeToStringFunc)(TokenType)) {
    bool success = expected == actual;
    printResult(success, testName);
    if (!success) {
      std::cerr << YELLOW << "Expected token type: " << RESET
                << tokenTypeToStringFunc(expected) << "\n"
                << CYAN << "Actual token type:   " << RESET
                << tokenTypeToStringFunc(actual) << "\n";
    }
  }
};

#endif // TEST_FRAMEWORK_HPP
