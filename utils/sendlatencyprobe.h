#ifndef SENDLATENCYPROBE_H
#define SENDLATENCYPROBE_H

#include <QString>
#include <QtGlobal>
#include <atomic>

// ══════════════════════════════════════════════════════════════════════════
// 串口发送延迟探针
//
// ── 测量口径 ─────────────────────────────────────────────────────────────
//   T1 = 主线程里人工点击发送按钮、进入构帧函数的那一刻
//   T2 = 工作线程里该次请求**第一次**真正 write 到 QSerialPort 的那一刻
//   发送延迟 = T2 - T1
//
//   只统计人工点击。自动发送源（10 分钟时间同步帧、导引头偏差像素帧等）
//   不打点：它们的写入在主线程找不到配对的点击令牌，落到 ignored 计数里，
//   不会进入样本。
//
//   两个打点都取本机单调时钟（QElapsedTimer::nsecsSinceReference），
//   跨线程一致，不需要对时。
//
// ── 使能开关（默认关闭，不用时对原功能无影响）────────────────────────────
//   编译期总闸 SENDLAT_COMPILED
//     1（默认）打点代码编进二进制，运行时默认关闭，靠环境变量启用
//     0        探针完全不参与编译：sendlatencyprobe.cpp 编译成空文件，
//              打点调用点编译成空函数，二进制里不留任何探针代码
//   运行时开关（环境变量优先于编译期默认值）
//     KYLIN_SENDLAT=1           启动即启用（1/true/on/yes）
//     KYLIN_SENDLAT=0           关闭（默认值）
//     KYLIN_SENDLAT_VERBOSE=0   关闭逐条打印（默认开启，每次点击打印一行）
//     KYLIN_SENDLAT_EVERY=50    每积累 N 个样本打印一次汇总，0=不打印
//     KYLIN_SENDLAT_CSV=路径    把原始样本落盘成 CSV
//   运行中可随时调用 SendLat::setEnabled(true/false)；
//   进程退出时自动打印一次汇总。
//
//   关闭状态下热路径上只有一次 relaxed 原子读，随即返回。
// ══════════════════════════════════════════════════════════════════════════

#ifndef SENDLAT_COMPILED
#define SENDLAT_COMPILED 1
#endif

#ifndef SENDLAT_DEFAULT_ON
#define SENDLAT_DEFAULT_ON 0
#endif

namespace SendLat {

// 被测量的串口通道。新增通道时在这里加一项，并在 .cpp 的 kChannelKeys
// 与 channelName() 里补上名字。
enum Channel {
    Image = 0,      // 图像导引头
    Laser,          // 激光导引头
    Turntable,      // 转台
    ChannelCount
};

const char *channelName(int ch);

// ── 使能开关 ─────────────────────────────────────────────────────────────
// 全局开关的存储定义在 .cpp。这里用 inline 包装，关闭时热路径上只有一次
// relaxed 原子读，随后立刻返回，不构造任何对象、不加锁、不分配内存。
extern std::atomic<bool> g_enabled;

inline bool enabled() { return g_enabled.load(std::memory_order_relaxed); }

bool envEnabled();          // 解析 KYLIN_SENDLAT（进程内只读一次）
void setEnabled(bool on);   // 运行中开关；切换时会清空配对状态
void install();             // 读环境变量 + 注册退出汇总，进程启动时自动调用

// ── 打点 ─────────────────────────────────────────────────────────────────
// markClick    主线程：人工点击入口（构帧函数首行）
// markSend     工作线程：真正 write 之前，本次请求的第一次产生样本
// discardClick 工作线程：请求被丢弃（串口未打开等），弹出令牌避免错配
#if SENDLAT_COMPILED
void markClickImpl(int ch);
void markSendImpl(int ch);
void discardClickImpl(int ch);

inline void markClick(int ch)    { if (enabled()) markClickImpl(ch); }
inline void markSend(int ch)     { if (enabled()) markSendImpl(ch); }
inline void discardClick(int ch) { if (enabled()) discardClickImpl(ch); }
#else
// SENDLAT_COMPILED=0：整个探针不参与编译，除了下面三个空函数之外，
// enabled() / setEnabled() / stats() / dumpSummary() 等接口都不存在，
// 别处若引用它们会链接失败——这正是"彻底剥离"应有的表现。
inline void markClick(int)    {}
inline void markSend(int)     {}
inline void discardClick(int) {}
#endif

// ── 统计 ─────────────────────────────────────────────────────────────────
struct Stats {
    qint64 samples   = 0;   // 有效样本数（= 人工点击次数）
    qint64 coalesced = 0;   // 同一次点击的后续帧（图像/激光补发拍）
    qint64 ignored   = 0;   // 没有对应点击的写入（自动源 / 其它手动入口）
    qint64 dropped   = 0;   // 请求被丢弃（串口未打开等）
    qint64 stale     = 0;   // 过期令牌（请求丢失后残留）
    double avgMs = 0.0;
    double p50Ms = 0.0;
    double p95Ms = 0.0;
    double p99Ms = 0.0;
    double minMs = 0.0;
    double maxMs = 0.0;
};

Stats stats(int ch);
void resetStats();
void dumpSummary();                 // 每个有数据的通道打印一行汇总
void setCsvPath(const QString &path);

} // namespace SendLat

#endif // SENDLATENCYPROBE_H
