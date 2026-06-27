#pragma once

#include <QObject>

/*!
 * brief 手柄数据处理工作类（工作线程对象）
 *
 * 职责：接收 Handle 发来的信号通知，执行实际的数据处理、协议解析、状态计算等。
 * 设计为 moveToThread 运行在后台线程，不阻塞主线程 UI。
 * 用法：
 *   Handle handle;
 *   GamepadProcessor *processor = new GamepadProcessor;
 *   QThread *thread = new QThread;
 *   processor->moveToThread(thread);
 *   handle.connectToProcessor(processor);
 *   connect(thread, &QThread::started, processor, &GamepadProcessor::dowork);
 *   thread->start();
 */
class GamepadProcessor : public QObject
{
    Q_OBJECT

public:
    explicit GamepadProcessor(QObject *parent = nullptr);
    ~GamepadProcessor();

    /*!
     * brief 线程启动后的初始化入口（与 SerialPortLaser::dowork 模式一致）
     */
    void dowork();

public slots:
    // ── 连接状态 ──
    void onConnected(int deviceId);
    void onDisconnected();

    // ── 按键 ──
    void onButtonA(bool pressed);
    void onButtonB(bool pressed);
    void onButtonX(bool pressed);
    void onButtonY(bool pressed);
    void onButtonL1(bool pressed);
    void onButtonR1(bool pressed);
    void onButtonL3(bool pressed);
    void onButtonR3(bool pressed);
    void onButtonStart(bool pressed);
    void onButtonSelect(bool pressed);
    void onButtonGuide(bool pressed);

    // ── 摇杆单轴 ──
    void onAxisLeftX(double value);
    void onAxisLeftY(double value);
    void onAxisRightX(double value);
    void onAxisRightY(double value);

    // ── 扳机 ──
    void onTriggerL2(double value);
    void onTriggerR2(double value);

signals:
    /*!
     * brief 处理完成后向外通知（可连接到主线程数据对象）
     */
    void inputProcessed(const QString &eventType, double value);

private:
    bool m_connected = false;
};
