# 使用 C++23 从零实现 RISC-V 模拟器

使用 C++23 从零实现的 RISC-V 模拟器，最终的模拟器可以运行 [xv6](https://pdos.csail.mit.edu/6.828/2012/xv6.html) 操作系统。

- 教程: [everystep](https://github.com/weijiew/everystep)

### 开发环境

- Linux / Win(WSL) / MacOS

使用 docker 开发，其内部包含了一个 Ubuntu 20.04，其中用到了 `riscv64-unknown-elf-gcc`， 也可以用 clang 来编译。

lab0-start-code 分支是一个包含了 gtest 的启动代码。项目名称为 remu ，即 Rapid Emulate 的缩写。

### 问题

如有任何问题，可以随时提 [issue](https://github.com/weijiew/cemu/issues)。