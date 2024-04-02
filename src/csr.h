//
// Created by Jie Wei on 2024/2/13.
//

#pragma once
#include <iostream>
#include <iomanip>
#include <array>
#include "param.h"

namespace crvemu {

class Csr {
public:
  Csr();  // 构造函数
  void dump_csrs() const;  // 打印所有的CSR
  uint64_t load(size_t addr) const;  // 加载指定地址的CSR
  void store(size_t addr, uint64_t value);  // 存储值到指定地址的CSR
  bool is_medelegated(uint64_t cause) const;  // 检查是否有机器异常委托
  bool is_midelegated(uint64_t cause) const;  // 检查是否有机器中断委托

private:
  std::array<uint64_t, NUM_CSRS> csrs;  // 存储CSR的数组
};

}
