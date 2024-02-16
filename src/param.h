#include <cstddef> // 引入定义 std::size_t 的头文件

namespace crvemu {

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

}