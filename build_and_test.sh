#!/bin/bash

# 运行 cmake 和 make
cmake -S . -B build
make -C build

# 运行所有测试

# 设置构建目录
BUILD_DIR="build/tests"

# 找到所有以 test_ 开头的可执行文件并运行它们
for test_file in ${BUILD_DIR}/test_*; do
  if [[ -x "$test_file" ]]; then
    echo "Running $test_file"
    $test_file
  else
    echo "$test_file is not executable or not found"
  fi
done
