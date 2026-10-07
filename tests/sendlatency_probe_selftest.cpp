// 发送延迟探针自检程序（不参与主程序构建，需要时手工编译运行）
//
// 编译（MinGW + Qt5，Qt 路径按本机改；写成一行，避免注释续行）：
//   g++ -std=gnu++14 -I<Qt>/include -I<Qt>/include/QtCore
//       tests/sendlatency_probe_selftest.cpp utils/sendlatencyprobe.cpp
//       -L<Qt>/lib -lQt5Core -o sendlat_selftest.exe
//
// 验证的是配对规则本身（样本数/补发帧/未配对/丢弃/过期），不验证绝对时延。

#include "../utils/sendlatencyprobe.h"

#include <QCoreApplication>
#include <QThread>
#include <cstdio>
#include <cstring>

static int g_failures = 0;

static void check(bool ok, const char *what)
{
    std::printf("%s %s\n", ok ? "[PASS]" : "[FAIL]", what);
    if (!ok) ++g_failures;
}

static void sleepMs(int ms) { QThread::msleep(static_cast<unsigned long>(ms)); }

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    SendLat::setEnabled(true);

    check(SendLat::ChannelCount == 3, "通道枚举覆盖 image / laser / turntable");
    check(std::strcmp(SendLat::channelName(SendLat::Turntable), "turntable") == 0,
          "通道名映射正确");

    // ── 场景1：一次点击 + 10 拍补发，只应产生 1 个样本，其余 9 拍计入补发帧 ──
    {
        SendLat::resetStats();
        SendLat::markClick(SendLat::Image);
        sleepMs(20);
        for (int i = 0; i < 10; ++i) {
            SendLat::markSend(SendLat::Image);
            sleepMs(20);
        }
        const SendLat::Stats st = SendLat::stats(SendLat::Image);
        check(st.samples == 1, "场景1: 10 拍只产生 1 个样本");
        check(st.coalesced == 9, "场景1: 其余 9 拍计入补发帧");
        check(st.avgMs >= 15.0 && st.avgMs <= 200.0, "场景1: 延迟落在 20ms 量级");
    }

    // ── 场景2：没有对应点击的写入（自动发送源）→ 未配对，且不产生样本 ──
    {
        SendLat::resetStats();
        SendLat::markSend(SendLat::Laser);
        const SendLat::Stats st = SendLat::stats(SendLat::Laser);
        check(st.samples == 0 && st.ignored == 1,
              "场景2: 自动发送源计入未配对且不产生样本");
    }

    // ── 场景3：请求被丢弃 → 计入丢弃，且不影响后续点击的配对 ──
    {
        SendLat::resetStats();
        SendLat::markClick(SendLat::Image);
        SendLat::discardClick(SendLat::Image);
        const SendLat::Stats st1 = SendLat::stats(SendLat::Image);
        check(st1.dropped == 1 && st1.samples == 0, "场景3: 丢弃的请求计入丢弃计数");

        SendLat::markClick(SendLat::Image);
        sleepMs(20);
        SendLat::markSend(SendLat::Image);
        const SendLat::Stats st2 = SendLat::stats(SendLat::Image);
        check(st2.samples == 1, "场景3: 丢弃后新的点击仍能正常配对");
        check(st2.avgMs >= 15.0 && st2.avgMs <= 200.0,
              "场景3: 采样值取的是本次点击的间隔，没有串到上一次");
    }

    // ── 场景4：超过过期阈值的令牌被丢弃，不产生错误样本 ──
    {
        SendLat::resetStats();
        SendLat::markClick(SendLat::Laser);
        sleepMs(600);                       // 超过 500ms 的过期阈值
        SendLat::markSend(SendLat::Laser);
        const SendLat::Stats st = SendLat::stats(SendLat::Laser);
        check(st.samples == 0 && st.stale == 1, "场景4: 过期令牌计入过期，不产生样本");
    }

    // ── 场景5：连续多次点击，每次各出一个样本 ──
    {
        SendLat::resetStats();
        for (int k = 0; k < 3; ++k) {
            SendLat::markClick(SendLat::Laser);
            sleepMs(10);
            for (int i = 0; i < 10; ++i) SendLat::markSend(SendLat::Laser);
            sleepMs(10);
        }
        const SendLat::Stats st = SendLat::stats(SendLat::Laser);
        check(st.samples == 3, "场景5: 3 次点击产生 3 个样本");
    }

    // ── 场景6：闩锁过期很久之后的自动帧，应计为未配对而不是补发帧 ──
    {
        SendLat::resetStats();
        SendLat::markClick(SendLat::Image);
        sleepMs(10);
        SendLat::markSend(SendLat::Image);      // 取样，闩锁生效
        SendLat::markSend(SendLat::Image);      // 紧邻的补发拍
        const SendLat::Stats a = SendLat::stats(SendLat::Image);
        check(a.samples == 1 && a.coalesced == 1 && a.ignored == 0,
              "场景6: 紧邻的补发拍计入补发帧");

        sleepMs(1100);                          // 超过 1s 的闩锁有效期
        SendLat::markSend(SendLat::Image);      // 相当于很久之后的自动帧
        const SendLat::Stats b = SendLat::stats(SendLat::Image);
        check(b.ignored == 1 && b.coalesced == 1,
              "场景6: 闩锁过期后的自动帧计入未配对，不污染补发帧计数");
    }

    // ── 场景7：转台一次点击发多帧（三轴联动 / 开转台），只取第一帧 ──
    {
        SendLat::resetStats();
        SendLat::markClick(SendLat::Turntable);
        sleepMs(5);
        SendLat::markSend(SendLat::Turntable);   // 第一帧 → 取样
        SendLat::markSend(SendLat::Turntable);   // 第二帧
        SendLat::markSend(SendLat::Turntable);   // 第三帧
        const SendLat::Stats st = SendLat::stats(SendLat::Turntable);
        check(st.samples == 1, "场景7: 转台一次点击只产生 1 个样本");
        check(st.coalesced == 2, "场景7: 转台后续两帧计入补发帧");
        check(st.avgMs >= 0.0 && st.avgMs <= 200.0, "场景7: 转台延迟量级合理");
    }

    // ── 场景8：关闭开关后打点无效 ──
    {
        SendLat::setEnabled(false);
        SendLat::resetStats();
        SendLat::markClick(SendLat::Image);
        SendLat::markSend(SendLat::Image);
        const SendLat::Stats st = SendLat::stats(SendLat::Image);
        check(st.samples == 0 && st.ignored == 0 && st.coalesced == 0,
              "场景8: 关闭状态下打点不产生任何计数");
    }

    std::printf("\n%s (%d failures)\n",
                g_failures ? "FAILED" : "ALL PASSED", g_failures);
    return g_failures ? 1 : 0;
}
