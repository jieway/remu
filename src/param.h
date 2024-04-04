#pragma once

#include <cstddef> // 引入定义 std::size_t 的头文件

namespace cemu {

// 使用 std::size_t 替代 uint64_t 来表示内存大小，更好地表达意图和增加可移植性
constexpr std::size_t DRAM_BASE = 0x8000'0000;

// 定义DRAM的大小，128MB
constexpr std::size_t DRAM_SIZE = 1024 * 1024 * 128;

// 定义DRAM的结束地址
constexpr std::size_t DRAM_END = DRAM_SIZE + DRAM_BASE - 1;

constexpr size_t NUM_CSRS = 4096;  // 定义CSR的数量为4096

// 机器级别的CSR
constexpr size_t MHARTID = 0xf14;  // 硬件线程ID
constexpr size_t MSTATUS = 0x300;  // 机器状态寄存器
constexpr size_t MEDELEG = 0x302;  // 机器异常委托寄存器
constexpr size_t MIDELEG = 0x303;  // 机器中断委托寄存器
constexpr size_t MIE = 0x304;  // 机器中断使能寄存器
constexpr size_t MTVEC = 0x305;  // 机器陷阱处理程序基地址
constexpr size_t MCOUNTEREN = 0x306;  // 机器计数器使能
constexpr size_t MSCRATCH = 0x340;  // 机器陷阱处理程序的临时寄存器
constexpr size_t MEPC = 0x341;  // 机器异常程序计数器
constexpr size_t MCAUSE = 0x342;  // 机器陷阱原因
constexpr size_t MTVAL = 0x343;  // 机器错误地址或指令
constexpr size_t MIP = 0x344;  // 机器中断挂起

// 监管级别的CSR
constexpr size_t SSTATUS = 0x100;  // 监管状态寄存器
constexpr size_t SIE = 0x104;  // 监管中断使能寄存器
constexpr size_t STVEC = 0x105;  // 监管陷阱处理程序基地址
constexpr size_t SSCRATCH = 0x140;  // 监管陷阱处理程序的临时寄存器
constexpr size_t SEPC = 0x141;  // 监管异常程序计数器
constexpr size_t SCAUSE = 0x142;  // 监管陷阱原因
constexpr size_t STVAL = 0x143;  // 监管错误地址或指令
constexpr size_t SIP = 0x144;  // 监管中断挂起
constexpr size_t SATP = 0x180;  // 监管地址转换和保护

// mstatus 和 sstatus 字段掩码
constexpr uint64_t MASK_SIE = 1 << 1;  // 监管中断使能掩码
constexpr uint64_t MASK_MIE = 1 << 3;  // 机器中断使能掩码
constexpr uint64_t MASK_SPIE = 1 << 5;  // 上一次监管中断使能掩码
constexpr uint64_t MASK_UBE = 1 << 6;  // 用户模式字节顺序掩码
constexpr uint64_t MASK_MPIE = 1 << 7;  // 上一次机器中断使能掩码
constexpr uint64_t MASK_SPP = 1 << 8;  // 上一次监管权限模式掩码
constexpr uint64_t MASK_VS = 0b11 << 9;  // 虚拟化状态掩码
constexpr uint64_t MASK_MPP = 0b11 << 11;  // 上一次机器权限模式掩码
constexpr uint64_t MASK_FS = 0b11 << 13;  // 浮点单元状态掩码
constexpr uint64_t MASK_XS = 0b11 << 15;  // 用户扩展状态掩码
constexpr uint64_t MASK_MPRV = 1 << 17;  // 内存保护寄存器掩码
constexpr uint64_t MASK_SUM = 1 << 18;  // 监管用户内存访问掩码
constexpr uint64_t MASK_MXR = 1 << 19;  // 内存扩展寄存器掩码
constexpr uint64_t MASK_TVM = 1 << 20;  // 虚拟化内存掩码
constexpr uint64_t MASK_TW = 1 << 21;  // 虚拟化等待掩码
constexpr uint64_t MASK_TSR = 1 << 22;  // 虚拟化SR掩码
constexpr uint64_t MASK_UXL = 0b11ULL << 32;  // 用户地址长度掩码
constexpr uint64_t MASK_SXL = 0b11ULL << 34;  // 监管地址长度掩码
constexpr uint64_t MASK_SBE = 1ULL << 36;  // 监管错误掩码
constexpr uint64_t MASK_MBE = 1ULL << 37;  // 机器错误掩码
constexpr uint64_t MASK_SD = 1ULL << 63;  // 状态脏掩码
constexpr uint64_t MASK_SSTATUS = MASK_SIE | MASK_SPIE | MASK_UBE | MASK_SPP | MASK_FS
                                | MASK_XS  | MASK_SUM  | MASK_MXR | MASK_UXL | MASK_SD;  // 监管状态寄存器掩码

// MIP / SIP 字段掩码
constexpr uint64_t MASK_SSIP = 1 << 1;  // 监管软件中断挂起掩码
constexpr uint64_t MASK_MSIP = 1 << 3;  // 机器软件中断挂起掩码
constexpr uint64_t MASK_STIP = 1 << 5;  // 监管定时器中断挂起掩码
constexpr uint64_t MASK_MTIP = 1 << 7;  // 机器定时器中断挂起掩码
constexpr uint64_t MASK_SEIP = 1 << 9;  // 监管外部中断挂起掩码
constexpr uint64_t MASK_MEIP = 1 << 11;  // 机器外部中断挂起掩码

// 使用 uint64_t 定义 Mode 类型
using Mode = uint64_t;

// 定义 User 模式，二进制表示为 00
constexpr Mode User = 0b00;

// 定义 Supervisor 模式，二进制表示为 01
constexpr Mode Supervisor = 0b01;

// 定义 Machine 模式，二进制表示为 11
constexpr Mode Machine = 0b11;

// PLIC的基地址，所有的PLIC寄存器都从这个地址开始映射到内存中。
constexpr uint64_t PLIC_BASE = 0xc000000;

// PLIC的大小，表示PLIC寄存器在内存中的映射区域的大小。这个大小是0x4000000。
constexpr uint64_t PLIC_SIZE = 0x4000000;

// PLIC的结束地址，表示PLIC寄存器在内存中的映射区域的结束地址。
constexpr uint64_t PLIC_END = PLIC_BASE + PLIC_SIZE - 1;

// PLIC的挂起寄存器的地址，当有中断挂起时，对应的位会被设置为1。
constexpr uint64_t PLIC_PENDING = PLIC_BASE + 0x1000;

// PLIC的使能寄存器的地址，可以通过设置这个寄存器来使能或禁止中断。
constexpr uint64_t PLIC_SENABLE = PLIC_BASE + 0x2000;

// PLIC的优先级寄存器的地址，可以通过设置这个寄存器来改变中断的优先级。
constexpr uint64_t PLIC_SPRIORITY = PLIC_BASE + 0x201000;

// PLIC的确认寄存器的地址，当处理器处理完一个中断后，
// 会写这个寄存器来通知PLIC中断已经被处理。
constexpr uint64_t PLIC_SCLAIM = PLIC_BASE + 0x201004;

// CLINT的基地址，所有的CLINT寄存器都从这个地址开始映射到内存中。
constexpr uint64_t CLINT_BASE = 0x2000000;

// CLINT的大小，表示CLINT寄存器在内存中的映射区域的大小。这个大小是0x10000。
constexpr uint64_t CLINT_SIZE = 0x10000;

// CLINT的结束地址，表示CLINT寄存器在内存中的映射区域的结束地址。
constexpr uint64_t CLINT_END = CLINT_BASE + CLINT_SIZE - 1;

// CLINT的mtimecmp寄存器的地址，可以通过设置这个寄存器来设置中断的触发时间。
constexpr uint64_t CLINT_MTIMECMP = CLINT_BASE + 0x4000;

// CLINT的mtime寄存器的地址，表示当前的时间。
constexpr uint64_t CLINT_MTIME = CLINT_BASE + 0xbff8;


// UART的基地址，所有的UART寄存器都从这个地址开始映射到内存中。
constexpr uint64_t UART_BASE = 0x10000000;

// UART的大小，表示UART寄存器在内存中的映射区域的大小。这个大小是0x100。
constexpr uint64_t UART_SIZE = 0x100;

// UART的结束地址，表示UART寄存器在内存中的映射区域的结束地址。
constexpr uint64_t UART_END = UART_BASE + UART_SIZE - 1;

// UART的中断号，表示UART的中断号。
constexpr uint64_t UART_IRQ = 10;

// UART的接收寄存器的地址，可以通过这个寄存器来读取接收到的数据。
constexpr uint64_t UART_RHR = 0;

// UART的发送寄存器的地址，可以通过这个寄存器来发送数据。
constexpr uint64_t UART_THR = 0;

// UART的线控寄存器的地址，可以通过这个寄存器来设置波特率等参数。
constexpr uint64_t UART_LCR = 3;

// 线状态寄存器。
// LSR BIT 0：
//     0 = 接收保持寄存器或FIFO中没有数据。
//     1 = 已接收并保存在接收保持寄存器或FIFO中的数据。
// LSR BIT 5：
//     0 = 发送保持寄存器已满。16550将不接受任何要传输的数据。
//     1 = 发送保持寄存器（或FIFO）为空。CPU可以加载下一个字符。
constexpr uint64_t UART_LSR = 5;

// 接收寄存器空
constexpr uint8_t MASK_UART_LSR_RX = 1;

// 发送寄存器空
constexpr uint8_t MASK_UART_LSR_TX = 1 << 5;
}