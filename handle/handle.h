#pragma once

#include <QObject>
#include <QGamepad>
#include <QGamepadManager>

class GamepadProcessor;

/*!
 * brief 游戏手柄控制器封装类（主线程对象）
 *
 * 职责：包装 Qt QGamepad，将原生信号转换为统一通知信号。
 * 不包含数据处理逻辑 —— 数据变动的实际处理交给 GamepadProcessor（工作线程）。
 */
class Handle : public QObject
{
    Q_OBJECT

public:
    explicit Handle(int deviceId = 0, QObject *parent = nullptr);
    ~Handle();

    bool isConnected() const { return m_isConnected; }
    int deviceId() const  { return m_deviceId; }

    /*!
     * brief 触发手柄震动（需要硬件支持）
     * param leftMotor  左马达强度 (0.0 ~ 1.0)
     * param rightMotor 右马达强度 (0.0 ~ 1.0)
     */
    Q_INVOKABLE void setVibration(double leftMotor, double rightMotor);

    /*!
     * brief 将 Handle 的信号连接到 GamepadProcessor 的槽（QueuedConnection 跨线程安全）
     */
    void connectToProcessor(GamepadProcessor *processor);

signals:
    // ==================== 连接状态信号 ====================
    void connected(int deviceId);
    void disconnected();

    // ==================== 按键信号（按下 true / 抬起 false） ====================
    void buttonAChanged(bool pressed);
    void buttonBChanged(bool pressed);
    void buttonXChanged(bool pressed);
    void buttonYChanged(bool pressed);
    void buttonL1Changed(bool pressed);
    void buttonR1Changed(bool pressed);
    void buttonL3Changed(bool pressed);
    void buttonR3Changed(bool pressed);
    void buttonStartChanged(bool pressed);
    void buttonSelectChanged(bool pressed);
    void buttonGuideChanged(bool pressed);

    // ==================== 轴/摇杆信号（值范围：-1.0 ~ 1.0） ====================
    void leftStickChanged(double x, double y);
    void rightStickChanged(double x, double y);
    void triggerChanged(double left, double right);

    // ==================== 单轴信号（便于按需连接） ====================
    void axisLeftXChanged(double value);
    void axisLeftYChanged(double value);
    void axisRightXChanged(double value);
    void axisRightYChanged(double value);
    void triggerL2Changed(double value);
    void triggerR2Changed(double value);

private slots:
    // 连接状态
    void onConnectedChanged();

    // 按钮
    void onButtonAChanged(bool pressed);
    void onButtonBChanged(bool pressed);
    void onButtonXChanged(bool pressed);
    void onButtonYChanged(bool pressed);
    void onButtonL1Changed(bool pressed);
    void onButtonR1Changed(bool pressed);
    void onButtonL3Changed(bool pressed);
    void onButtonR3Changed(bool pressed);
    void onButtonStartChanged(bool pressed);
    void onButtonSelectChanged(bool pressed);
    void onButtonGuideChanged(bool pressed);

    // 摇杆单轴
    void onAxisLeftXChanged(double value);
    void onAxisLeftYChanged(double value);
    void onAxisRightXChanged(double value);
    void onAxisRightYChanged(double value);

    // 扳机
    void onButtonL2Changed(double value);
    void onButtonR2Changed(double value);

private:
    void connectGamepadSignals();

    QGamepad *m_gamepad;
    int       m_deviceId;
    bool      m_isConnected;

    // 摇杆缓存（用于合成 leftStickChanged / rightStickChanged）
    double    m_leftX  = 0.0;
    double    m_leftY  = 0.0;
    double    m_rightX = 0.0;
    double    m_rightY = 0.0;
    double    m_l2     = 0.0;
    double    m_r2     = 0.0;
};
