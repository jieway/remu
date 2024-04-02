//
// Created by Jie Wei on 2024/4/2.
//

#pragma once
#include <cstdint>
#include <iostream>
#include <sstream>

namespace crvemu {

enum class ExceptionType {
  InstructionAddrMisaligned,  // 指令地址不对齐
  InstructionAccessFault,     // 指令访问错误
  IllegalInstruction,         // 非法指令
  Breakpoint,                 // 断点
  LoadAccessMisaligned,       // 加载地址不对齐
  LoadAccessFault,            // 加载访问错误
  StoreAMOAddrMisaligned,     // 存储/原子操作地址不对齐
  StoreAMOAccessFault,        // 存储/原子操作访问错误
  EnvironmentCallFromUMode,   // 用户态环境调用
  EnvironmentCallFromSMode,   // 监管态环境调用
  EnvironmentCallFromMMode,   // 机器态环境调用
  InstructionPageFault,       // 指令页错误
  LoadPageFault,              // 加载页错误
  StoreAMOPageFault,          // 存储/原子操作页错误
};

class Exception {
 public:
  Exception(ExceptionType type, uint64_t value);  // 构造函数

  uint64_t getValue() const;                      // 获取异常值

  ExceptionType getType() const;                  // 获取异常类型

  bool isFatal() const;                           // 是否是致命异常

  std::string what() const;                       // 异常信息

  // 重载输出流
  friend std::ostream& operator<<(std::ostream& os, const Exception& e);

 private:
  ExceptionType type; // 异常类型
  uint64_t value;     // 异常值
};

}