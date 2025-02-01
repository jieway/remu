# RISC-V 模拟器

一个使用 C++23 从零实现的 RISC-V 模拟器，目前已支持部分基础指令（如 ADDI 和 ADD），最终目标是运行 [xv6](https://pdos.csail.mit.edu/6.828/2012/xv6.html) 操作系统。

## 项目结构

```
.
├── src/           # 源代码目录，包含 CPU 模拟器核心实现
├── tests/         # 测试代码目录，内置单元测试框架
├── build/         # 构建输出目录
└── add-addi.s     # 示例汇编文件，用于验证 ADDI 和 ADD 指令的实现
```

## 开发环境

- 支持平台：Linux / Windows(WSL) / macOS
- 编译器要求：支持 C++23 标准
- 构建工具：CMake 3.15+
- 开发环境：Docker (Ubuntu 24.04 基础镜像)

## 快速开始

1. 克隆仓库：
```bash
git clone https://github.com/weijiew/remu.git
cd remu
```

2. 使用 Docker 启动开发环境：
```bash
./run.sh
```

3. 编译并运行：
```bash
./compile_and_exec.sh
```

在编译和运行过程中，模拟器会加载指定的二进制文件（例如由 `add-addi.s` 汇编得到的 `add-addi.bin`），执行时会输出日志以及 CPU 状态信息以便调试。

## 项目特性

- 使用现代 C++23 特性实现
- 支持部分 RISC-V 指令（如 ADDI 和 ADD），后续会逐步扩展至完整指令集
- 内置单元测试框架，确保关键功能正确性
- Docker 容器化的开发环境，简化跨平台配置与部署
- 支持交叉编译

## 开发进度

- [x] 基础项目框架搭建
- [ ] CPU 核心结构设计（初步实现，部分指令支持中）
- [ ] 指令解码器实现（目前已支持 ADDI 和 ADD）
- [ ] 内存管理单元
- [ ] 中断处理机制
- [ ] xv6 支持

## 问题反馈

如有任何问题或建议，欢迎提交 [Issue](https://github.com/weijiew/cemu/issues)。

## 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件