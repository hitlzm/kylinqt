#include "handle.h"
#include "gamepadprocessor.h"
#include <QGamepadManager>
// ═══════════════════════════════════════════════════════════
// 构造 / 析构
// ═══════════════════════════════════════════════════════════

Handle::Handle(int deviceId, QObject *parent)
    : QObject(parent)
    , m_gamepad(new QGamepad(deviceId, this))      //初始化操作搬到连接函数中
    , m_deviceId(deviceId)
    , m_isConnected(false)
{   
    //检查可用手柄
    
    connectGamepadSignals();
    onConnectedChanged();   // 初始化时检查一次连接状态
}

Handle::~Handle() {}

// ═══════════════════════════════════════════════════════════
// 公开方法
// ═══════════════════════════════════════════════════════════

void Handle::setVibration(double leftMotor, double rightMotor)
{
    Q_UNUSED(leftMotor);
    Q_UNUSED(rightMotor);
    // QGamepad::setVibration 在 Qt 5.15+ 才可用
    // 当前 Qt 5.12.8 不支持振动 API
}

void Handle::connectToProcessor(GamepadProcessor *processor)
{
    if (!processor) return;

    // 连接状态
    connect(this, &Handle::connected,    processor, &GamepadProcessor::onConnected,    Qt::QueuedConnection);
    connect(this, &Handle::disconnected, processor, &GamepadProcessor::onDisconnected, Qt::QueuedConnection);

    // 按键
    connect(this, &Handle::buttonAChanged,      processor, &GamepadProcessor::onButtonA,      Qt::QueuedConnection);
    connect(this, &Handle::buttonBChanged,      processor, &GamepadProcessor::onButtonB,      Qt::QueuedConnection);
    connect(this, &Handle::buttonXChanged,      processor, &GamepadProcessor::onButtonX,      Qt::QueuedConnection);
    connect(this, &Handle::buttonYChanged,      processor, &GamepadProcessor::onButtonY,      Qt::QueuedConnection);
    connect(this, &Handle::buttonL1Changed,     processor, &GamepadProcessor::onButtonL1,     Qt::QueuedConnection);
    connect(this, &Handle::buttonR1Changed,     processor, &GamepadProcessor::onButtonR1,     Qt::QueuedConnection);
    connect(this, &Handle::buttonL3Changed,     processor, &GamepadProcessor::onButtonL3,     Qt::QueuedConnection);
    connect(this, &Handle::buttonR3Changed,     processor, &GamepadProcessor::onButtonR3,     Qt::QueuedConnection);
    connect(this, &Handle::buttonStartChanged,  processor, &GamepadProcessor::onButtonStart,  Qt::QueuedConnection);
    connect(this, &Handle::buttonSelectChanged, processor, &GamepadProcessor::onButtonSelect, Qt::QueuedConnection);
    connect(this, &Handle::buttonGuideChanged,  processor, &GamepadProcessor::onButtonGuide,  Qt::QueuedConnection);

    // 摇杆（单轴，精确处理）
    connect(this, &Handle::axisLeftXChanged,  processor, &GamepadProcessor::onAxisLeftX,  Qt::QueuedConnection);
    connect(this, &Handle::axisLeftYChanged,  processor, &GamepadProcessor::onAxisLeftY,  Qt::QueuedConnection);
    connect(this, &Handle::axisRightXChanged, processor, &GamepadProcessor::onAxisRightX, Qt::QueuedConnection);
    connect(this, &Handle::axisRightYChanged, processor, &GamepadProcessor::onAxisRightY, Qt::QueuedConnection);

    // 扳机
    connect(this, &Handle::triggerL2Changed, processor, &GamepadProcessor::onTriggerL2, Qt::QueuedConnection);
    connect(this, &Handle::triggerR2Changed, processor, &GamepadProcessor::onTriggerR2, Qt::QueuedConnection);
}

// ═══════════════════════════════════════════════════════════
// 内部：一次性连接所有 QGamepad 原生信号 → Handle 私有槽
// ═══════════════════════════════════════════════════════════

void Handle::connectGamepadSignals()
{
    connect(m_gamepad, &QGamepad::connectedChanged,    this, &Handle::onConnectedChanged);

    // 按键
    connect(m_gamepad, &QGamepad::buttonAChanged,      this, &Handle::onButtonAChanged);
    connect(m_gamepad, &QGamepad::buttonBChanged,      this, &Handle::onButtonBChanged);
    connect(m_gamepad, &QGamepad::buttonXChanged,      this, &Handle::onButtonXChanged);
    connect(m_gamepad, &QGamepad::buttonYChanged,      this, &Handle::onButtonYChanged);
    connect(m_gamepad, &QGamepad::buttonL1Changed,     this, &Handle::onButtonL1Changed);
    connect(m_gamepad, &QGamepad::buttonR1Changed,     this, &Handle::onButtonR1Changed);
    connect(m_gamepad, &QGamepad::buttonL3Changed,     this, &Handle::onButtonL3Changed);
    connect(m_gamepad, &QGamepad::buttonR3Changed,     this, &Handle::onButtonR3Changed);
    connect(m_gamepad, &QGamepad::buttonStartChanged,  this, &Handle::onButtonStartChanged);
    connect(m_gamepad, &QGamepad::buttonSelectChanged, this, &Handle::onButtonSelectChanged);
    connect(m_gamepad, &QGamepad::buttonGuideChanged,  this, &Handle::onButtonGuideChanged);

    // 摇杆单轴
    connect(m_gamepad, &QGamepad::axisLeftXChanged,  this, &Handle::onAxisLeftXChanged);
    connect(m_gamepad, &QGamepad::axisLeftYChanged,  this, &Handle::onAxisLeftYChanged);
    connect(m_gamepad, &QGamepad::axisRightXChanged, this, &Handle::onAxisRightXChanged);
    connect(m_gamepad, &QGamepad::axisRightYChanged, this, &Handle::onAxisRightYChanged);

    // 扳机
    connect(m_gamepad, &QGamepad::buttonL2Changed, this, &Handle::onButtonL2Changed);
    connect(m_gamepad, &QGamepad::buttonR2Changed, this, &Handle::onButtonR2Changed);

    // D-Pad — Qt 5.12 的 QGamepad 无原生 D-Pad 信号，由摇杆轴模拟
}

// ═══════════════════════════════════════════════════════════
// 私有槽：QGamepad 信号 → Handle 信号（通知层，不做数据处理）
// ═══════════════════════════════════════════════════════════

void Handle::onConnectedChanged()
{
    m_isConnected = m_gamepad->isConnected();
    if (m_isConnected)
        emit connected(m_deviceId);
    else
        emit disconnected();
}

// ── 按键：直接转发 ──

void Handle::onButtonAChanged(bool pressed)      { emit buttonAChanged(pressed); }
void Handle::onButtonBChanged(bool pressed)      { emit buttonBChanged(pressed); }
void Handle::onButtonXChanged(bool pressed)      { emit buttonXChanged(pressed); }
void Handle::onButtonYChanged(bool pressed)      { emit buttonYChanged(pressed); }
void Handle::onButtonL1Changed(bool pressed)     { emit buttonL1Changed(pressed); }
void Handle::onButtonR1Changed(bool pressed)     { emit buttonR1Changed(pressed); }
void Handle::onButtonL3Changed(bool pressed)     { emit buttonL3Changed(pressed); }
void Handle::onButtonR3Changed(bool pressed)     { emit buttonR3Changed(pressed); }
void Handle::onButtonStartChanged(bool pressed)  { emit buttonStartChanged(pressed); }
void Handle::onButtonSelectChanged(bool pressed) { emit buttonSelectChanged(pressed); }
void Handle::onButtonGuideChanged(bool pressed)  { emit buttonGuideChanged(pressed); }

// ── 摇杆单轴：缓存 + 发送单轴信号 + 发送合成信号 ──

void Handle::onAxisLeftXChanged(double value)
{
    m_leftX = value;
    emit axisLeftXChanged(value);
    emit leftStickChanged(m_leftX, m_leftY);
}

void Handle::onAxisLeftYChanged(double value)
{
    m_leftY = value;
    emit axisLeftYChanged(value);
    emit leftStickChanged(m_leftX, m_leftY);
}

void Handle::onAxisRightXChanged(double value)
{
    m_rightX = value;
    emit axisRightXChanged(value);
    emit rightStickChanged(m_rightX, m_rightY);
}

void Handle::onAxisRightYChanged(double value)
{
    m_rightY = value;
    emit axisRightYChanged(value);
    emit rightStickChanged(m_rightX, m_rightY);
}

// ── 扳机：单轴信号 + 合成信号 ──

void Handle::onButtonL2Changed(double value)
{
    m_l2 = value;
    emit triggerL2Changed(value);
    emit triggerChanged(m_l2, m_r2);
}

void Handle::onButtonR2Changed(double value)
{
    m_r2 = value;
    emit triggerR2Changed(value);
    emit triggerChanged(m_l2, m_r2);
}
