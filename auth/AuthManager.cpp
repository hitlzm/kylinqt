#include "AuthManager.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QMessageAuthenticationCode>

// ═══ 盐与摘要 ═══════════════════════════════════════════════════════════
// 由 tools/genkey.py 离线生成（HMAC-SHA256(盐, 口令) 的十六进制结果）。
// 换口令：重跑 genkey.py，只替换 kKeyDigestHex 这一行；kKeySalt 不要改动，
//         盐一变摘要全部失效。
// 摘要可以公开（它就在二进制里），口令本身不应出现在任何文件、日志或注释中。
namespace {
const char *const kKeySalt      = "8x102FOBMtL8NuYZ";
const char *const kKeyDigestHex = "7260ba6aaa02647d957583fd2b10c978d7cb55dea3c950cd5f7c829e3a4d7c84";

// 逐字节比较：不用 operator== 是为了让比较耗时与"前多少字节相同"无关
bool constantTimeEquals(const QByteArray &a, const QByteArray &b)
{
    if (a.size() != b.size())
        return false;
    unsigned char diff = 0;
    for (int i = 0; i < a.size(); ++i)
        diff |= static_cast<unsigned char>(a.at(i)) ^ static_cast<unsigned char>(b.at(i));
    return diff == 0;
}
} // namespace

AuthManager::AuthManager(QObject *parent)
    : QObject(parent)
{
    const int expectedBytes = QCryptographicHash::hashLength(QCryptographicHash::Sha256);
    if (QByteArray::fromHex(kKeyDigestHex).size() != expectedBytes) {
        qWarning() << "[Auth] kKeyDigestHex 长度异常（应为 64 个十六进制字符），"
                      "口令将无法通过校验，请用 tools/genkey.py 重新生成";
    }

    m_lockTimer.setInterval(1000);
    connect(&m_lockTimer, &QTimer::timeout, this, &AuthManager::tickLockout);
}

bool AuthManager::verify(const QString &input)
{
    if (m_authenticated)
        return true;

    if (m_lockSecondsLeft > 0) {
        emit verifyFailed(tr("尝试次数已用尽，请等待 %1 秒").arg(m_lockSecondsLeft));
        return false;
    }

    // 首尾空白通常是复制粘贴或误触带入的，不参与比对（口令本身不含空白）
    const QString key = input.trimmed();
    if (key.isEmpty()) {
        emit verifyFailed(tr("请输入密钥"));
        return false;   // 空输入不计入失败次数
    }

    const QByteArray digest = QMessageAuthenticationCode::hash(
        key.toUtf8(),
        QByteArray(kKeySalt),
        QCryptographicHash::Sha256);

    if (constantTimeEquals(digest, QByteArray::fromHex(kKeyDigestHex))) {
        m_authenticated = true;
        m_failedAttempts = 0;
        qInfo() << "[Auth] 校验通过";
        emit attemptsLeftChanged();
        emit authenticatedChanged();
        emit loginSucceeded();
        return true;
    }

    ++m_failedAttempts;
    emit attemptsLeftChanged();
    qWarning() << "[Auth] 校验失败，累计" << m_failedAttempts << "次";

    if (m_failedAttempts >= kMaxAttempts) {
        startLockout();
        emit verifyFailed(tr("密钥错误，尝试次数已用尽，请等待 %1 秒").arg(m_lockSecondsLeft));
    } else {
        emit verifyFailed(tr("密钥错误，还可尝试 %1 次").arg(attemptsLeft()));
    }
    return false;
}

void AuthManager::giveUp()
{
    qInfo() << "[Auth] 用户取消登录，退出程序";
    QCoreApplication::quit();
}

void AuthManager::beginStartup()
{
    if (!m_authenticated) {
        qWarning() << "[Auth] 未通过校验就调用 beginStartup，已忽略";
        return;
    }
    if (m_startupBegun)
        return;
    m_startupBegun = true;
    qInfo() << "[Auth] 开始启动串口与手柄线程";
    emit startupRequested();
}

void AuthManager::startLockout()
{
    m_lockSecondsLeft = kLockMilliseconds / 1000;
    m_lockTimer.start();
    emit lockSecondsLeftChanged();
}

void AuthManager::tickLockout()
{
    if (m_lockSecondsLeft > 0)
        --m_lockSecondsLeft;

    if (m_lockSecondsLeft <= 0) {
        m_lockTimer.stop();
        m_failedAttempts = 0;   // 解锁后重新获得尝试次数
        emit attemptsLeftChanged();
    }
    emit lockSecondsLeftChanged();
}
