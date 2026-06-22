#pragma once

#include <QObject>
#include <QGamepad>
#include <QGamepadManager>

/*!
 * brief 游戏手柄控制器封装类
 * 自动连接系统中第一个可用手柄，并发射标准的按键/摇杆信号
 */
class Handle : public QObject
{
    Q_OBJECT

public:
    /*!
     * brief 构造函数，默认连接设备ID 0（通常为第一个手柄）
     * param parent 父对象
     * param deviceId 手柄设备ID，默认为0
     */
    explicit Handle(int deviceId = 0, QObject *parent = nullptr);
    ~Handle();

    /*!
     * brief 检查手柄是否已连接
     * return true 表示已连接
     */
    bool isConnected() const { return m_isConnected; }

    /*!
     * brief 获取当前绑定的设备ID
     * return 设备ID，-1 表示无设备
     */
    int deviceId() const { return m_deviceId; }

    /*!
     * \brief 触发手柄震动（需要硬件支持）
     * \param weak 左马达强度 (0.0 ~ 1.0)
     * \param strong 右马达强度 (0.0 ~ 1.0)
     */
    void setVibration(double weak, double strong);

signals:
    // ==================== 连接状态信号 ====================
    void deviceConnected(int id);
    void deviceDisconnected();

    // ==================== 按键信号（布尔值：按下 true / 抬起 false） ====================
    void buttonAChanged(bool pressed);
    void buttonBChanged(bool pressed);
    void buttonXChanged(bool pressed);
    void buttonYChanged(bool pressed);
    void buttonL1Changed(bool pressed);   // 左肩键
    void buttonR1Changed(bool pressed);   // 右肩键
    void buttonL3Changed(bool pressed);   // 左摇杆按下
    void buttonR3Changed(bool pressed);   // 右摇杆按下
    void buttonStartChanged(bool pressed);
    void buttonSelectChanged(bool pressed);
    void buttonGuideChanged(bool pressed); // Xbox 西瓜键 / PS Home键

    // ==================== 轴/摇杆信号（值范围：-1.0 ~ 1.0） ====================
    void leftStickChanged(double x, double y);   // 左摇杆
    void rightStickChanged(double x, double y);  // 右摇杆
    void triggerChanged(double left, double right); // 左右扳机 (0.0 ~ 1.0)

private slots:
    // 内部连接 QGamepad 信号的槽函数
    void onConnectedChanged();
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
    void onAxisLeftX(double value);
    void onAxisLeftY(double value);
    void onAxisRightX(double value);
    void onAxisRightY(double value);
    void onButtonL2(double value);  // 注意：L2/R2 在 Qt 中按扳机轴处理
    void onButtonR2(double value);

private:
    QGamepad *m_gamepad;    // Qt游戏手柄核心对象
    int       m_deviceId;   // 绑定的设备ID
    bool      m_isConnected; // 连接状态缓存
};