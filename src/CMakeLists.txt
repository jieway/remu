# 🎯 创建可执行文件目标（Executable Target）
# 📦 add_executable 命令用于声明一个可执行文件的构建目标
# 🏷️ 第一个参数 remu 是目标名称（最终生成的可执行文件名）
# 📄 后续参数是构建所需的源文件列表（这里只有一个 main.cpp）
# 🌈 完整语法：add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL] [source1] [source2 ...])
add_executable(remu main.cpp 
                    cpu.cpp)   # 👷♂️ 构建引擎会执行以下操作：
                               # 1️⃣ 编译 main.cpp -> main.obj（Windows）或 main.o（Unix）
                               # 2️⃣ 链接生成 remu.exe（Windows）或 remu（Unix）
                               # 🚀 后续可用 target_link_libraries 添加库依赖
                               # 🛠️ 调试建议：使用 set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 生成 compile_commands.json