#ifndef PRECISETIMER_H
#define PRECISETIMER_H

#include <QObject>
#include <memory>

/**
 * @brief 高精度周期定时器，API 与 QTimer 兼容，可直接替换。
 *
 * 跨平台实现：
 *  - Linux：  timerfd + QSocketNotifier（内核 hrtimer，抖动 < ±1ms）
 *  - Windows：内部退回到 QTimer（开发调试可用，部署在麒麟上自动获得高精度）
 *
 * 用法（与 QTimer 完全一致）：
 * @code
 *   PreciseTimer *t = new PreciseTimer(this);
 *   t->setInterval(40);
 *   connect(t, &PreciseTimer::timeout, this, &MyClass::onTick);
 *   t->start();
 *   // ...
 *   t->stop();
 * @endcode
 */
class PreciseTimer : public QObject
{
    Q_OBJECT

public:
    explicit PreciseTimer(QObject *parent = nullptr);
    ~PreciseTimer() override;

    void setInterval(int msec);
    int  interval() const { return m_intervalMs; }

    bool isActive() const;

    int  remainingTime() const;

public slots:
    void start();
    void start(int msec);
    void stop();

signals:
    void timeout();

private:
    // PIMPL：隐藏平台相关实现细节，头文件保持干净
    struct PlatformImpl;
    std::unique_ptr<PlatformImpl> m_impl;

    int  m_intervalMs = 0;
};

#endif // PRECISETIMER_H
