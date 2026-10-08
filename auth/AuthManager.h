#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

/**
 * @brief 启动登录校验（HMAC-SHA256 摘要比对）
 *
 * 程序里只保存口令的 HMAC 摘要，不保存口令本身。登录时把用户输入按同一算法算一遍再与
 * 摘要比对，因此源码/二进制中的摘要泄漏不等于口令泄漏：摘要不能当口令使用（把摘要本身
 * 输进来会算出另一个值）。摘要由 tools/genkey.py 离线生成，换口令时重新生成并替换
 * AuthManager.cpp 里的 kKeyDigestHex，盐 kKeySalt 保持不变。
 *
 * 连续失败 kMaxAttempts 次后锁定 kLockMilliseconds 毫秒，锁定期间不受理任何输入。
 * 注意：失败次数只存在内存里，重启程序即清零。
 *
 * 全部方法在主线程使用（QML 直接调用）。
 */
class AuthManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool authenticated READ authenticated NOTIFY authenticatedChanged)
    Q_PROPERTY(int attemptsLeft READ attemptsLeft NOTIFY attemptsLeftChanged)
    Q_PROPERTY(int lockSecondsLeft READ lockSecondsLeft NOTIFY lockSecondsLeftChanged)

public:
    static constexpr int kMaxAttempts = 5;
    static constexpr int kLockMilliseconds = 60000;

    explicit AuthManager(QObject *parent = nullptr);

    bool authenticated() const { return m_authenticated; }
    int attemptsLeft() const { return kMaxAttempts - m_failedAttempts; }
    int lockSecondsLeft() const { return m_lockSecondsLeft; }

    // 校验输入口令。成功返回 true 并发出 authenticated()，失败发出 verifyFailed()
    Q_INVOKABLE bool verify(const QString &input);
    // 登录页"退出"按钮
    Q_INVOKABLE void giveUp();
    // 主界面实例化完成后由 QML 调用，通知 C++ 启动串口/手柄线程等硬件相关部分
    Q_INVOKABLE void beginStartup();

signals:
    void authenticatedChanged();
    void attemptsLeftChanged();
    void lockSecondsLeftChanged();
    // 校验通过（与只读属性 authenticated 区分开命名，避免与属性读取函数重名）
    void loginSucceeded();
    void verifyFailed(const QString &reason);
    void startupRequested();

private:
    void startLockout();
    void tickLockout();

    bool m_authenticated = false;
    bool m_startupBegun = false;
    int m_failedAttempts = 0;
    int m_lockSecondsLeft = 0;
    QTimer m_lockTimer;
};
