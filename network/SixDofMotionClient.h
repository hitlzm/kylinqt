#ifndef SIXDOFMOTIONCLIENT_H
#define SIXDOFMOTIONCLIENT_H

#include <QByteArray>
#include <QObject>
#include <QString>

class QUdpSocket;
class QTimer;

/**
 * 六自由度转台 UDP 运动控制客户端（主线程，QML 可直接调用）
 *
 * 参照 "六自由度平台/motion_hmi_kylin" 下的 udp_client.py 与 protocol.py 实现，
 * 对应原 C# MainWindow.xaml.cs 的 InitSocket() / SendData() / SendDataStatue()：
 *
 *   1) 单一 UDP 套接字：bind 本机端口（默认 0.0.0.0:8080），收发共用一个 socket；
 *   2) 运动指令 → 192.168.15.255:7408（子网广播），56 字节定长帧，小端单精度：
 *        55 AA BB | cmd | tx ty tz | rx ry rz | 6×float 保留 | int32 time(ms)
 *      cmd=6 位置运行（定点/回中位/回底部共用），cmd=2 急停；
 *   3) 状态查询 → 255.255.255.255:8081，报文 0x55 0xAA 0x70，周期 200ms；
 *   4) 接收到的数据先按控制卡 IP（默认 192.168.15.201）过滤，收到应答即认为链路正常。
 *
 * 与参考实现保持一致的两处“坑”，不要自行补上：
 *   - 参考工程只用一个 socket，且 ModifyPort() 从未被调用，因此不发 IP 配置包（55 AA 31）；
 *   - 回中位/回底部并不使用 cmd=3/4，而是统一用 cmd=6（位置运行）+ 位置全零 / tz=-0.5，
 *     cmd=3/4 仅作为协议常量保留。
 *
 * 倾角控制（本次需求重点）：
 *   X 轴倾角 → rx（绕 X 轴，弧度），Y 轴倾角 → ry（绕 Y 轴，弧度），
 *   其余自由度（tx/ty/tz/rz）一律置 0，因此只改变倾角，不产生平移与偏航动作。
 *   对外接口角度用“度”，组包时换算为弧度，与 protocol.py 的 deg_to_rad() 一致。
 *
 * 线程模型：套接字、定时器、组包与发送全部在主线程完成，不使用工作线程，
 *          因此 QML 的按钮 / Timer 可以直接调用，不存在跨线程排队与竞争问题。
 */
class SixDofMotionClient : public QObject
{
    Q_OBJECT

    // ── 网络参数（对应 config.json 的 network 段）──
    Q_PROPERTY(int     localPort          READ localPort          WRITE setLocalPort          NOTIFY localPortChanged)
    Q_PROPERTY(QString targetAddress      READ targetAddress      WRITE setTargetAddress      NOTIFY targetAddressChanged)
    Q_PROPERTY(int     targetPort         READ targetPort         WRITE setTargetPort         NOTIFY targetPortChanged)
    Q_PROPERTY(QString statusQueryAddress READ statusQueryAddress WRITE setStatusQueryAddress NOTIFY statusQueryAddressChanged)
    Q_PROPERTY(int     statusQueryPort    READ statusQueryPort    WRITE setStatusQueryPort    NOTIFY statusQueryPortChanged)
    Q_PROPERTY(QString controlCardAddress READ controlCardAddress WRITE setControlCardAddress NOTIFY controlCardAddressChanged)
    Q_PROPERTY(bool    autoGoMiddleOnConnect READ autoGoMiddleOnConnect WRITE setAutoGoMiddleOnConnect NOTIFY autoGoMiddleOnConnectChanged)

    // ── 连接状态 ──
    Q_PROPERTY(bool    connected      READ connected      NOTIFY connectedChanged)       // 套接字是否已打开
    Q_PROPERTY(bool    linkAlive      READ linkAlive      NOTIFY linkAliveChanged)       // 是否收到过控制卡应答
    Q_PROPERTY(QString statusMessage  READ statusMessage  NOTIFY statusMessageChanged)   // 最近一次状态/报警描述
    Q_PROPERTY(int     statusCode     READ statusCode     NOTIFY statusCodeChanged)      // 最近一次状态/报警码
    Q_PROPERTY(QString lastError      READ lastError      NOTIFY lastErrorChanged)       // 最近一次错误描述

    // ── 运动参数 ──
    Q_PROPERTY(int    executeTimeMs READ executeTimeMs WRITE setExecuteTime NOTIFY executeTimeMsChanged) // 执行时间(ms)
    Q_PROPERTY(double tiltXDeg      READ tiltXDeg      NOTIFY tiltChanged)   // 最近一次下发的 X 轴倾角(°)
    Q_PROPERTY(double tiltYDeg      READ tiltYDeg      NOTIFY tiltChanged)   // 最近一次下发的 Y 轴倾角(°)

public:
    // ── 协议常量（与 protocol.py 一一对应）──
    static constexpr quint8  kStart1 = 0x55;
    static constexpr quint8  kStart2 = 0xAA;
    static constexpr quint8  kStart3 = 0xBB;

    static constexpr quint8  kCmdInit      = 1;   // 初始化
    static constexpr quint8  kCmdEstop     = 2;   // 急停
    static constexpr quint8  kCmdGoMiddle  = 3;   // 回初始/中位（协议保留，当前用 cmd=6 实现）
    static constexpr quint8  kCmdGoBottom  = 4;   // 到底部（协议保留，当前用 cmd=6 实现）
    static constexpr quint8  kCmdLoopRun   = 5;   // 循环运行
    static constexpr quint8  kCmdPosRun    = 6;   // 位置运行（定点/回中位/回底部全用 6）

    static constexpr int     kFrameSize    = 56;  // 3 起始 + cmd + 12 float + int32
    static constexpr quint16 kDefaultTargetPort  = 7408;
    static constexpr quint16 kDefaultStatusPort  = 8081;
    static constexpr int     kDefaultLocalPort   = 8080;
    static constexpr int     kStatusQueryIntervalMs = 200;  // 对应 C# tmrProgress.Interval=200ms
    static constexpr int     kLinkTimeoutMs         = 1500; // 超过该时间无应答即判定链路中断

    // 各指令的默认执行时间（与参考实现逐条对应，未调用 setExecuteTime 时生效）：
    //   定点运动/倾角下发：main_window.py 中“执行时间”输入框默认 300
    //   回中位：udp_client.py go_middle() 默认 200
    //   回底部：udp_client.py go_bottom() 默认 300
    //   急停：udp_client.py estop() 固定 300
    static constexpr int     kDefaultMoveTimeMs   = 300;
    static constexpr int     kDefaultMiddleTimeMs = 200;
    static constexpr int     kDefaultBottomTimeMs = 300;
    static constexpr int     kDefaultEstopTimeMs  = 300;

    // 倾角限位：rx（X 轴倾角）、ry（Y 轴倾角）最大 ±10°，超限直接拒绝发送（不做静默限幅）。
    // 注：参考工程 main_window.py 的 AXIS_LIMITS / sine_controller.py 的 LIMITS_ANGLE_DEG
    //     原为 ±33°，本项目按现场要求收紧到 ±10°。
    static constexpr double  kMaxTiltDeg = 10.0;

    explicit SixDofMotionClient(QObject *parent = nullptr);
    ~SixDofMotionClient() override;

    // ── 属性读取 ──
    int     localPort() const          { return m_localPort; }
    QString targetAddress() const      { return m_targetAddress; }
    int     targetPort() const         { return m_targetPort; }
    QString statusQueryAddress() const { return m_statusQueryAddress; }
    int     statusQueryPort() const    { return m_statusQueryPort; }
    QString controlCardAddress() const { return m_controlCardAddress; }
    bool    autoGoMiddleOnConnect() const { return m_autoGoMiddleOnConnect; }
    bool    connected() const          { return m_connected; }
    bool    linkAlive() const          { return m_linkAlive; }
    QString statusMessage() const      { return m_statusMessage; }
    int     statusCode() const         { return m_statusCode; }
    QString lastError() const          { return m_lastError; }
    int     executeTimeMs() const      { return m_executeTimeMs; }
    double  tiltXDeg() const           { return m_tiltXDeg; }
    double  tiltYDeg() const           { return m_tiltYDeg; }

    // ── 属性设置（网络参数建议在未连接时修改）──
    void setLocalPort(int port);
    void setTargetAddress(const QString &address);
    void setTargetPort(int port);
    void setStatusQueryAddress(const QString &address);
    void setStatusQueryPort(int port);
    void setControlCardAddress(const QString &address);
    void setAutoGoMiddleOnConnect(bool enabled);

    // ══════════════════════════════════════════════════════════
    // 预留接口 1：网口连接 + 连接成功判断
    // ══════════════════════════════════════════════════════════
    Q_INVOKABLE bool openConnection();    // 打开套接字并启动状态轮询，返回是否打开成功
    Q_INVOKABLE void closeConnection();   // 关闭套接字与轮询
    Q_INVOKABLE bool isConnected() const;      // 套接字是否已打开
    Q_INVOKABLE bool checkConnection() const;  // 连接成功判断：套接字已打开 且 收到控制卡应答
    Q_INVOKABLE void startConnectionCheck(int timeoutMs = 3000); // 异步判定，结果经 connectionCheckFinished 返回

    // ══════════════════════════════════════════════════════════
    // X / Y 轴倾角控制
    // ══════════════════════════════════════════════════════════
    Q_INVOKABLE bool sendTiltXY(double xDeg, double yDeg);                          // 用当前执行时间
    Q_INVOKABLE bool sendTiltXYWithTime(double xDeg, double yDeg, int timeMs);      // 指定本次执行时间

    // ══════════════════════════════════════════════════════════
    // 预留接口 2：执行时间设置（帧尾 int32 time 字段，单位 ms）
    // 未调用前，各指令沿用参考实现各自的默认执行时间；调用后全部改用该值。
    // ══════════════════════════════════════════════════════════
    Q_INVOKABLE void setExecuteTime(int msec);

    // ══════════════════════════════════════════════════════════
    // 预留接口 3：回中位 / 回底部
    // ══════════════════════════════════════════════════════════
    Q_INVOKABLE bool goMiddle();                     // 回中位：六轴全部归零
    Q_INVOKABLE bool goMiddleWithTime(int timeMs);
    Q_INVOKABLE bool goBottom();                     // 回底部：tz=-0.5m，其余归零
    Q_INVOKABLE bool goBottomWithTime(int timeMs);

    // 急停（对应 udp_client.py 的 estop()，cmd=2）
    Q_INVOKABLE bool emergencyStop();

signals:
    void localPortChanged();
    void targetAddressChanged();
    void targetPortChanged();
    void statusQueryAddressChanged();
    void statusQueryPortChanged();
    void controlCardAddressChanged();
    void autoGoMiddleOnConnectChanged();

    void connectedChanged();
    void linkAliveChanged();
    void statusMessageChanged();
    void statusCodeChanged();
    void lastErrorChanged();

    void executeTimeMsChanged();
    void tiltChanged();

    void connectionCheckFinished(bool success, const QString &message);
    void commandSent(const QString &description);

private slots:
    void onSocketReadyRead();
    void onStatusTimerTimeout();
    void onCheckTimerTimeout();

private:
    // ── 一帧运动指令（对应 protocol.py 的 RecMsg，共 56 字节）──
    struct MoveFrame
    {
        quint8 cmd = kCmdPosRun;
        float  tx = 0.0f, ty = 0.0f, tz = 0.0f;   // 平移，单位米
        float  rx = 0.0f, ry = 0.0f, rz = 0.0f;   // 角度，单位弧度
        float  tax = 0.0f, tay = 0.0f, taz = 0.0f;
        float  rvx = 0.0f, rvy = 0.0f, rvz = 0.0f;
        qint32 time = kDefaultMoveTimeMs;         // 执行时间，单位 ms
    };

    static QByteArray buildFrame(const MoveFrame &frame);
    static double degToRad(double deg);
    static QString statusText(int code);

    // 未显式设置执行时间时用各指令自己的默认值，设置过则统一使用 m_executeTimeMs
    int effectiveTime(int commandDefaultMs) const
    {
        return m_executeTimeOverridden ? m_executeTimeMs : commandDefaultMs;
    }

    bool transmit(const MoveFrame &frame, const QString &description);
    void sendStatusQuery();
    void parseStatus(const QByteArray &datagram);

    void setConnected(bool connected);
    void setLinkAlive(bool alive);
    void setStatusCode(int code);
    void setStatusMessage(const QString &message);
    void setLastError(const QString &message);

    QUdpSocket *m_socket = nullptr;
    QTimer     *m_statusTimer = nullptr;   // 200ms 状态查询 / 链路看门狗
    QTimer     *m_checkTimer = nullptr;    // 连接成功判断的一次性超时定时器

    // 网络参数
    int     m_localPort = kDefaultLocalPort;
    QString m_targetAddress = QStringLiteral("192.168.15.255");       // 子网广播
    int     m_targetPort = kDefaultTargetPort;
    QString m_statusQueryAddress = QStringLiteral("255.255.255.255"); // 全网广播
    int     m_statusQueryPort = kDefaultStatusPort;
    QString m_controlCardAddress = QStringLiteral("192.168.15.201");
    bool    m_autoGoMiddleOnConnect = false;  // 参考实现在连接时自动回中位，默认关闭以免误动作

    // 连接状态
    bool   m_connected = false;
    bool   m_linkAlive = false;
    qint64 m_lastStatusRxMs = 0;   // 最近一次收到控制卡应答的本机时刻

    // 运动参数
    int    m_executeTimeMs = kDefaultMoveTimeMs;
    bool   m_executeTimeOverridden = false;   // 用户是否调用过 setExecuteTime()
    double m_tiltXDeg = 0.0;
    double m_tiltYDeg = 0.0;

    QString m_statusMessage;
    QString m_lastError;
    int     m_statusCode = 0;
};

#endif // SIXDOFMOTIONCLIENT_H
