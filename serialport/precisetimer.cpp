#include "precisetimer.h"

#include <QDebug>
#include <QTimer>
#include <cerrno>
#include <cstring>

#ifdef __linux__
#include <QSocketNotifier>
#include <unistd.h>
#include <sys/timerfd.h>
#endif

// ══════════════════════════════════════════════════════════════
// 平台相关实现（PIMPL）
// ══════════════════════════════════════════════════════════════

struct PreciseTimer::PlatformImpl
{
#ifdef __linux__
    int              timerFd  = -1;
    QSocketNotifier *notifier = nullptr;

    void create(int intervalMs, PreciseTimer *owner);
    void destroy();
    int  remainingTime() const;

#else  // Windows / other — 使用 QTimer 作为回退
    QTimer *fallback = nullptr;

    void create(int intervalMs, PreciseTimer *owner);
    void destroy();
    int  remainingTime() const;
#endif
};

// ──────────────────────────────────────────────────────
// Linux 实现：timerfd + QSocketNotifier
// ──────────────────────────────────────────────────────
#ifdef __linux__

void PreciseTimer::PlatformImpl::create(int intervalMs, PreciseTimer *owner)
{
    timerFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerFd < 0) {
        qWarning("PreciseTimer: timerfd_create failed: %s", std::strerror(errno));
        return;
    }

    struct itimerspec its;
    its.it_value.tv_sec  = intervalMs / 1000;
    its.it_value.tv_nsec = (intervalMs % 1000) * 1000000L;
    its.it_interval.tv_sec  = intervalMs / 1000;
    its.it_interval.tv_nsec = (intervalMs % 1000) * 1000000L;
    timerfd_settime(timerFd, 0, &its, nullptr);

    notifier = new QSocketNotifier(timerFd, QSocketNotifier::Read, owner);
    QObject::connect(notifier, &QSocketNotifier::activated, owner, [owner](int fd) {
        uint64_t expirations = 0;
        ssize_t n = read(fd, &expirations, sizeof(expirations));
        if (n >= 0) {
            emit owner->timeout();
        } else {
            qWarning("PreciseTimer: read on timerfd failed: %s", std::strerror(errno));
        }
    });
    notifier->setEnabled(true);
}

void PreciseTimer::PlatformImpl::destroy()
{
    if (notifier) {
        notifier->setEnabled(false);
        delete notifier;
        notifier = nullptr;
    }
    if (timerFd >= 0) {
        close(timerFd);
        timerFd = -1;
    }
}

int PreciseTimer::PlatformImpl::remainingTime() const
{
    if (timerFd < 0)
        return 0;

    struct itimerspec its;
    if (timerfd_gettime(timerFd, &its) != 0)
        return 0;

    return static_cast<int>(its.it_value.tv_sec * 1000
                          + its.it_value.tv_nsec / 1000000);
}

// ──────────────────────────────────────────────────────
// Windows / 其他平台回退：普通 QTimer
// ──────────────────────────────────────────────────────
#else

void PreciseTimer::PlatformImpl::create(int intervalMs, PreciseTimer *owner)
{
    fallback = new QTimer(owner);
    fallback->setInterval(intervalMs);
    QObject::connect(fallback, &QTimer::timeout, owner, &PreciseTimer::timeout);
    fallback->start();
}

void PreciseTimer::PlatformImpl::destroy()
{
    if (fallback) {
        fallback->stop();
        delete fallback;
        fallback = nullptr;
    }
}

int PreciseTimer::PlatformImpl::remainingTime() const
{
    return fallback ? fallback->remainingTime() : 0;
}

#endif  // __linux__

// ══════════════════════════════════════════════════════════════
// PreciseTimer 公共 API（平台无关）
// ══════════════════════════════════════════════════════════════

PreciseTimer::PreciseTimer(QObject *parent)
    : QObject(parent)
    , m_impl(std::make_unique<PlatformImpl>())
{
}

PreciseTimer::~PreciseTimer()
{
    stop();
}

void PreciseTimer::setInterval(int msec)
{
    m_intervalMs = msec;
}

void PreciseTimer::start()
{
    if (isActive())
        return;

    m_impl->create(m_intervalMs, this);
}

void PreciseTimer::start(int msec)
{
    stop();
    setInterval(msec);
    start();
}

void PreciseTimer::stop()
{
    m_impl->destroy();
}

bool PreciseTimer::isActive() const
{
#ifdef __linux__
    return m_impl->timerFd >= 0;
#else
    return m_impl->fallback && m_impl->fallback->isActive();
#endif
}

int PreciseTimer::remainingTime() const
{
    return m_impl->remainingTime();
}
