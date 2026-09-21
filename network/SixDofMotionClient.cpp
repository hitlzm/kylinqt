// winsock2.h 必须在任何 windows.h 之前包含，因此放在所有 Qt 头文件之前
#if defined(_WIN32)
#  include <winsock2.h>
#else
#  include <sys/socket.h>
#endif

#include "SixDofMotionClient.h"

#include <QAbstractSocket>
#include <QDataStream>
#include <QDebug>
#include <QDateTime>
#include <QHostAddress>
#include <QIODevice>
#include <QTimer>
#include <QUdpSocket>
#include <QtGlobal>

namespace {

// 一帧完整的运动指令：3 字节起始标志 + 1 字节命令 + 12 个单精度 float + 4 字节执行时间
constexpr int kMoveFrameSize = 56;

// 静止量：只做倾角控制时，位移与偏航一律置 0
constexpr float kZeroFloat = 0.0f;

// 回底部的纵向位移，单位米（与 udp_client.py 的 go_bottom() 一致：tz = -0.5m）
constexpr double kBottomTzMeters = -0.5;

// 显式置位 SO_BROADCAST：运动帧发往 192.168.15.255、状态查询发往 255.255.255.255，
// 两者都是广播地址，麒麟/Linux 下缺少该选项会直接返回 EACCES 而丢包。
void enableBroadcastOption(qintptr descriptor)
{
    if (descriptor < 0)
        return;

    const int enabled = 1;
#if defined(_WIN32)
    ::setsockopt(static_cast<SOCKET>(descriptor), SOL_SOCKET, SO_BROADCAST,
                 reinterpret_cast<const char *>(&enabled), sizeof(enabled));
#else
    ::setsockopt(static_cast<int>(descriptor), SOL_SOCKET, SO_BROADCAST,
                 &enabled, static_cast<socklen_t>(sizeof(enabled)));
#endif
}

} // namespace


SixDofMotionClient::SixDofMotionClient(QObject *parent)
    : QObject(parent)
{
    m_statusTimer = new QTimer(this);
    m_statusTimer->setTimerType(Qt::CoarseTimer);
    connect(m_statusTimer, &QTimer::timeout, this, &SixDofMotionClient::onStatusTimerTimeout);

    m_checkTimer = new QTimer(this);
    m_checkTimer->setSingleShot(true);
    connect(m_checkTimer, &QTimer::timeout, this, &SixDofMotionClient::onCheckTimerTimeout);
}

SixDofMotionClient::~SixDofMotionClient()
{
    // 套接字以 this 为父对象，析构时自动回收；这里只做必要的收尾
    if (m_socket) {
        m_socket->close();
        m_socket = nullptr;
    }
}


// ══════════════════════════════════════════════════════════════
// 属性设置
// ══════════════════════════════════════════════════════════════

void SixDofMotionClient::setLocalPort(int port)
{
    if (port <= 0 || port > 65535 || port == m_localPort)
        return;

    // 本机端口只在 bind 时生效，连接中修改需要重新打开连接
    m_localPort = port;
    emit localPortChanged();
}

void SixDofMotionClient::setTargetAddress(const QString &address)
{
    if (address.isEmpty() || address == m_targetAddress)
        return;
    m_targetAddress = address;
    emit targetAddressChanged();
}

void SixDofMotionClient::setTargetPort(int port)
{
    if (port <= 0 || port > 65535 || port == m_targetPort)
        return;
    m_targetPort = port;
    emit targetPortChanged();
}

void SixDofMotionClient::setStatusQueryAddress(const QString &address)
{
    if (address.isEmpty() || address == m_statusQueryAddress)
        return;
    m_statusQueryAddress = address;
    emit statusQueryAddressChanged();
}

void SixDofMotionClient::setStatusQueryPort(int port)
{
    if (port <= 0 || port > 65535 || port == m_statusQueryPort)
        return;
    m_statusQueryPort = port;
    emit statusQueryPortChanged();
}

void SixDofMotionClient::setControlCardAddress(const QString &address)
{
    if (address == m_controlCardAddress)
        return;
    m_controlCardAddress = address;
    emit controlCardAddressChanged();
}

void SixDofMotionClient::setAutoGoMiddleOnConnect(bool enabled)
{
    if (enabled == m_autoGoMiddleOnConnect)
        return;
    m_autoGoMiddleOnConnect = enabled;
    emit autoGoMiddleOnConnectChanged();
}


// ══════════════════════════════════════════════════════════════
// 预留接口 1：网口连接 / 断开 / 连接成功判断
// ══════════════════════════════════════════════════════════════

bool SixDofMotionClient::openConnection()
{
    if (m_socket && m_connected)
        return true;

    if (!m_socket) {
        m_socket = new QUdpSocket(this);
        connect(m_socket, &QUdpSocket::readyRead, this, &SixDofMotionClient::onSocketReadyRead);
    }

    // 与参考实现一致：绑定本机端口，收发共用一个 socket。
    // 先尝试可共享绑定（允许同机再开一个监听），失败再退回独占绑定。
    const quint16 port = static_cast<quint16>(m_localPort);
    bool bound = m_socket->bind(QHostAddress::AnyIPv4, port,
                                QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
    if (!bound)
        bound = m_socket->bind(QHostAddress::AnyIPv4, port);

    if (!bound) {
        setLastError(QStringLiteral("UDP 端口 %1 打开失败：%2")
                         .arg(m_localPort)
                         .arg(m_socket->errorString()));
        m_socket->deleteLater();
        m_socket = nullptr;
        setConnected(false);
        setLinkAlive(false);
        return false;
    }

    // 目标地址必须是合法 IP，否则发送会在运行时静默失败
    if (QHostAddress(m_targetAddress).isNull()) {
        setLastError(QStringLiteral("运动目标地址不合法：%1").arg(m_targetAddress));
        m_socket->close();
        m_socket->deleteLater();
        m_socket = nullptr;
        setConnected(false);
        setLinkAlive(false);
        return false;
    }

    // 运动帧与状态查询都发往广播地址，显式开启广播发送权限
    enableBroadcastOption(m_socket->socketDescriptor());

    m_lastStatusRxMs = 0;
    setLinkAlive(false);
    setConnected(true);
    setStatusMessage(QStringLiteral("套接字已打开：本机端口 %1 → 运动 %2:%3，状态查询 %4:%5")
                         .arg(m_localPort)
                         .arg(m_targetAddress)
                         .arg(m_targetPort)
                         .arg(m_statusQueryAddress)
                         .arg(m_statusQueryPort));

    // 启动 200ms 状态轮询（同时兼作链路看门狗）
    m_statusTimer->start(kStatusQueryIntervalMs);
    sendStatusQuery();

    // 参考实现在窗口加载后会自动回中位；这里默认关闭，需要时置 autoGoMiddleOnConnect 为 true
    if (m_autoGoMiddleOnConnect)
        goMiddle();

    return true;
}

void SixDofMotionClient::closeConnection()
{
    if (m_statusTimer)
        m_statusTimer->stop();
    if (m_checkTimer)
        m_checkTimer->stop();

    if (m_socket) {
        m_socket->close();
        m_socket->deleteLater();
        m_socket = nullptr;
    }

    setConnected(false);
    setLinkAlive(false);
    m_lastStatusRxMs = 0;
    setStatusMessage(QStringLiteral("UDP 连接已关闭"));
}

bool SixDofMotionClient::isConnected() const
{
    return m_connected;
}

// 连接成功判断：套接字已打开，并且收到过控制卡的状态应答。
// UDP 的 bind 成功并不代表控制卡在线，因此必须看到应答才算连接成功。
bool SixDofMotionClient::checkConnection() const
{
    return m_connected && m_linkAlive;
}

// 异步连接判定：timeoutMs 内收到控制卡应答即成功，超时未收到则失败。
// 不阻塞主线程，结果通过 connectionCheckFinished(bool, QString) 返回。
void SixDofMotionClient::startConnectionCheck(int timeoutMs)
{
    if (!m_connected && !openConnection()) {
        emit connectionCheckFinished(false, QStringLiteral("套接字打开失败"));
        return;
    }

    if (m_linkAlive) {
        emit connectionCheckFinished(true, QStringLiteral("已收到控制卡应答，连接正常"));
        return;
    }

    setStatusMessage(QStringLiteral("正在等待控制卡应答（%1ms）…").arg(timeoutMs));
    m_checkTimer->start(qMax(100, timeoutMs));
}


// ══════════════════════════════════════════════════════════════
// X / Y 轴倾角控制
// ══════════════════════════════════════════════════════════════

bool SixDofMotionClient::sendTiltXY(double xDeg, double yDeg)
{
    return sendTiltXYWithTime(xDeg, yDeg, effectiveTime(kDefaultMoveTimeMs));
}

// X 轴倾角 → rx（绕 X 轴），Y 轴倾角 → ry（绕 Y 轴），其余自由度置 0。
// 与参考实现一致使用 cmd=6（位置运行），角度由度换算为弧度。
bool SixDofMotionClient::sendTiltXYWithTime(double xDeg, double yDeg, int timeMs)
{
    // 与参考实现一致：超限直接拒绝发送，不做静默限幅，避免平台被驱到限位
    if (!qIsFinite(xDeg) || !qIsFinite(yDeg)
        || qAbs(xDeg) > kMaxTiltDeg || qAbs(yDeg) > kMaxTiltDeg) {
        setLastError(QStringLiteral("倾角超限：X=%1° Y=%2°（限位 ±%3°），指令未发送")
                         .arg(xDeg, 0, 'f', 3)
                         .arg(yDeg, 0, 'f', 3)
                         .arg(kMaxTiltDeg, 0, 'f', 1));
        return false;
    }

    MoveFrame frame;
    frame.cmd  = kCmdPosRun;
    frame.tx   = kZeroFloat;
    frame.ty   = kZeroFloat;
    frame.tz   = kZeroFloat;
    frame.rx   = static_cast<float>(degToRad(xDeg));
    frame.ry   = static_cast<float>(degToRad(yDeg));
    frame.rz   = kZeroFloat;
    frame.time = static_cast<qint32>(qMax(1, timeMs));

    const QString description = QStringLiteral("倾角指令 X=%1° Y=%2° 执行时间 %3ms")
                                    .arg(xDeg, 0, 'f', 3)
                                    .arg(yDeg, 0, 'f', 3)
                                    .arg(frame.time);

    if (!transmit(frame, description))
        return false;

    if (!qFuzzyCompare(m_tiltXDeg + 1.0, xDeg + 1.0)
        || !qFuzzyCompare(m_tiltYDeg + 1.0, yDeg + 1.0)) {
        m_tiltXDeg = xDeg;
        m_tiltYDeg = yDeg;
        emit tiltChanged();
    }
    return true;
}


// ══════════════════════════════════════════════════════════════
// 预留接口 2：执行时间设置
// ══════════════════════════════════════════════════════════════

// 执行时间字段（帧尾 int32，单位 ms），对应协议里每个动作的完成时间。
// 参考实现：定点/到顶部/到底部用 300ms、回中位用 200ms。这里提供一个统一下发口，
// 调用之后所有指令都改用该值（未调用则各自沿用参考实现的默认值）。
void SixDofMotionClient::setExecuteTime(int msec)
{
    const int clamped = qBound(1, msec, 60000);
    if (m_executeTimeOverridden && clamped == m_executeTimeMs)
        return;
    m_executeTimeMs = clamped;
    m_executeTimeOverridden = true;
    emit executeTimeMsChanged();
}


// ══════════════════════════════════════════════════════════════
// 预留接口 3：回中位 / 回底部
// ══════════════════════════════════════════════════════════════

// 回中位：六轴全部归零（udp_client.py 的 go_middle()：cmd=6、全零、time=200）
bool SixDofMotionClient::goMiddle()
{
    return goMiddleWithTime(effectiveTime(kDefaultMiddleTimeMs));
}

bool SixDofMotionClient::goMiddleWithTime(int timeMs)
{
    MoveFrame frame;
    frame.cmd  = kCmdPosRun;
    frame.time = static_cast<qint32>(qMax(1, timeMs));

    const QString description = QStringLiteral("回中位（六轴归零）执行时间 %1ms").arg(frame.time);
    return transmit(frame, description);
}

// 回底部：tz = -0.5m，其余归零（udp_client.py 的 go_bottom()：cmd=6、tz=-0.5）
bool SixDofMotionClient::goBottom()
{
    return goBottomWithTime(effectiveTime(kDefaultBottomTimeMs));
}

bool SixDofMotionClient::goBottomWithTime(int timeMs)
{
    MoveFrame frame;
    frame.cmd  = kCmdPosRun;
    frame.tz   = static_cast<float>(kBottomTzMeters);
    frame.time = static_cast<qint32>(qMax(1, timeMs));

    const QString description = QStringLiteral("回底部（tz=%1m）执行时间 %2ms")
                                    .arg(kBottomTzMeters, 0, 'f', 3)
                                    .arg(frame.time);
    return transmit(frame, description);
}

// 急停：cmd=2（udp_client.py 的 estop()）
bool SixDofMotionClient::emergencyStop()
{
    MoveFrame frame;
    frame.cmd  = kCmdEstop;
    frame.time = kDefaultEstopTimeMs;
    return transmit(frame, QStringLiteral("急停"));
}


// ══════════════════════════════════════════════════════════════
// 发送 / 组包
// ══════════════════════════════════════════════════════════════

bool SixDofMotionClient::transmit(const MoveFrame &frame, const QString &description)
{
    if (!m_socket || !m_connected) {
        setLastError(QStringLiteral("六自由度平台未连接，%1 未发送").arg(description));
        return false;
    }

    const QByteArray datagram = buildFrame(frame);
    const qint64 written = m_socket->writeDatagram(datagram,
                                                   QHostAddress(m_targetAddress),
                                                   static_cast<quint16>(m_targetPort));
    if (written != datagram.size()) {
        setLastError(QStringLiteral("%1 发送失败：%2").arg(description, m_socket->errorString()));
        return false;
    }

    emit commandSent(description);
    return true;
}

// 56 字节定长帧，小端单精度，与 protocol.py 的 REC_MSG_FORMAT = '<BBBBffffffffffffi' 一致：
//   55 AA BB | cmd | tx ty tz | rx ry rz | tax tay taz | rvx rvy rvz | int32 time
QByteArray SixDofMotionClient::buildFrame(const MoveFrame &frame)
{
    QByteArray datagram;
    datagram.reserve(kMoveFrameSize);

    QDataStream ds(&datagram, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);                  // C# 结构体直发，小端
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);  // float 占 4 字节

    ds << kStart1 << kStart2 << kStart3 << frame.cmd;
    ds << frame.tx << frame.ty << frame.tz;
    ds << frame.rx << frame.ry << frame.rz;
    ds << frame.tax << frame.tay << frame.taz;
    ds << frame.rvx << frame.rvy << frame.rvz;
    ds << frame.time;

    Q_ASSERT(datagram.size() == kMoveFrameSize);
    return datagram;
}

double SixDofMotionClient::degToRad(double deg)
{
    return deg * 3.141592653589793 / 180.0;
}


// ══════════════════════════════════════════════════════════════
// 状态轮询 / 接收 / 链路判定
// ══════════════════════════════════════════════════════════════

// 0x55 0xAA 0x70 → 255.255.255.255:8081（对应 SendDataStatue()，周期 200ms）
void SixDofMotionClient::sendStatusQuery()
{
    if (!m_socket || !m_connected)
        return;

    static const QByteArray kQueryPacket = QByteArray::fromHex("55AA70");
    m_socket->writeDatagram(kQueryPacket,
                            QHostAddress(m_statusQueryAddress),
                            static_cast<quint16>(m_statusQueryPort));
}

void SixDofMotionClient::onStatusTimerTimeout()
{
    sendStatusQuery();

    // 链路看门狗：超过 kLinkTimeoutMs 没有收到控制卡应答，判定链路中断
    if (m_linkAlive && m_lastStatusRxMs > 0) {
        const qint64 idleMs = QDateTime::currentMSecsSinceEpoch() - m_lastStatusRxMs;
        if (idleMs > kLinkTimeoutMs) {
            setLinkAlive(false);
            setStatusMessage(QStringLiteral("与控制卡失去联系（%1ms 无应答）").arg(idleMs));
        }
    }

    // 连接判定提前成功：不用等满超时时间
    if (m_checkTimer && m_checkTimer->isActive() && m_linkAlive) {
        m_checkTimer->stop();
        emit connectionCheckFinished(true, QStringLiteral("已收到控制卡应答，连接正常"));
    }
}

void SixDofMotionClient::onCheckTimerTimeout()
{
    const bool ok = m_connected && m_linkAlive;
    const QString message = ok ? QStringLiteral("已收到控制卡应答，连接正常")
                               : QStringLiteral("超时未收到控制卡应答，请检查控制卡 IP 与网络");
    setStatusMessage(message);
    emit connectionCheckFinished(ok, message);
}

// 接收控制卡应答：先按控制卡 IP 过滤（对应 C# SocketReceive 的 Contains 判断），
// 再把报文交给 parseStatus 解析状态码/报警码。
void SixDofMotionClient::onSocketReadyRead()
{
    while (m_socket && m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(m_socket->pendingDatagramSize()));

        QHostAddress sender;
        quint16 senderPort = 0;
        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        // 非控制卡（例如同网段其它设备的广播）直接丢弃，避免日志被刷屏
        if (!m_controlCardAddress.isEmpty() && sender.toString() != m_controlCardAddress)
            continue;

        m_lastStatusRxMs = QDateTime::currentMSecsSinceEpoch();
        if (!m_linkAlive)
            setLinkAlive(true);

        parseStatus(datagram);
    }
}

// 状态反馈报文（与 protocol.py 的 parse_status_response() 一致）：
//   0x64 0x03 <code> → 报警/错误
//   0xC9 0x3C <code> → 状态反馈
void SixDofMotionClient::parseStatus(const QByteArray &datagram)
{
    if (datagram.size() < 2)
        return;

    const quint8 first  = static_cast<quint8>(datagram.at(0));
    const quint8 second = static_cast<quint8>(datagram.at(1));
    const int code = datagram.size() >= 3 ? static_cast<quint8>(datagram.at(2)) : 0;

    if (first == 0x64 && second == 0x03) {
        setStatusCode(code);
        setStatusMessage(QStringLiteral("控制卡报警：%1").arg(statusText(code)));
        return;
    }

    if (first == 0xC9 && second == 0x3C) {
        setStatusCode(code);
        setStatusMessage(QStringLiteral("控制卡状态：%1").arg(statusText(code)));
    }
}

// 错误码中文描述（与 protocol.py 的 get_error_msg() 保持一致）
QString SixDofMotionClient::statusText(int code)
{
    switch (code) {
    case 100: return QStringLiteral("伺服错误，伺服未使能或参数错误");
    case 101: return QStringLiteral("伺服报警或未使能");
    case 102: return QStringLiteral("网线断开");
    case 103: return QStringLiteral("CAN总线错误");
    case 104: return QStringLiteral("UDP接收数据错误");
    case 110: return QStringLiteral("X轴不存在或未上电");
    case 111: return QStringLiteral("Y轴不存在或未上电");
    case 112: return QStringLiteral("Z轴不存在或未上电");
    case 113: return QStringLiteral("U轴不存在或未上电");
    case 114: return QStringLiteral("V轴不存在或未上电");
    case 115: return QStringLiteral("W轴不存在或未上电");
    case 120: return QStringLiteral("X轴报警或未使能");
    case 121: return QStringLiteral("Y轴报警或未使能");
    case 122: return QStringLiteral("Z轴报警或未使能");
    case 123: return QStringLiteral("U轴报警或未使能");
    case 124: return QStringLiteral("V轴报警或未使能");
    case 125: return QStringLiteral("W轴报警或未使能");
    case 1:   return QStringLiteral("运行中");
    case 2:   return QStringLiteral("急停");
    case 3:   return QStringLiteral("到达扭矩返回零位");
    case 4:   return QStringLiteral("复位");
    case 5:   return QStringLiteral("急停");
    case 6:   return QStringLiteral("运行");
    case 7:   return QStringLiteral("运行到最高点");
    case 8:   return QStringLiteral("IO点动");
    default:  return QStringLiteral("未知状态码 %1").arg(code);
    }
}


// ══════════════════════════════════════════════════════════════
// 内部状态维护
// ══════════════════════════════════════════════════════════════

void SixDofMotionClient::setConnected(bool connected)
{
    if (connected == m_connected)
        return;
    m_connected = connected;
    emit connectedChanged();
}

void SixDofMotionClient::setLinkAlive(bool alive)
{
    if (alive == m_linkAlive)
        return;
    m_linkAlive = alive;
    emit linkAliveChanged();
}

void SixDofMotionClient::setStatusCode(int code)
{
    if (code == m_statusCode)
        return;
    m_statusCode = code;
    emit statusCodeChanged();
}

void SixDofMotionClient::setStatusMessage(const QString &message)
{
    if (message == m_statusMessage)
        return;
    m_statusMessage = message;
    emit statusMessageChanged();
}

void SixDofMotionClient::setLastError(const QString &message)
{
    if (message != m_lastError) {
        m_lastError = message;
        emit lastErrorChanged();
    }
    // 不写入界面日志（本项目不需要六自由度平台的日志记录），仅在控制台留一条警告便于排查
    qWarning() << "[SixDofMotion]" << message;
}
