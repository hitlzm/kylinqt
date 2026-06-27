#include "gamepadprocessor.h"
#include <QDebug>


GamepadProcessor::GamepadProcessor(QObject *parent)
    : QObject(parent)
{
}

GamepadProcessor::~GamepadProcessor() {}


void GamepadProcessor::dowork()
{
    // 工作线程初始化逻辑（如果需要）
    qDebug() << "[GamepadProcessor] worker thread started";
}

// ═══════════════════════════════════════════════════════════
// 连接状态
// ═══════════════════════════════════════════════════════════

void GamepadProcessor::onConnected(int deviceId)
{
    m_connected = true;
    // TODO: 设备连接后的初始化逻辑
    qDebug() << "[GamepadProcessor] device connected:" << deviceId;
}

void GamepadProcessor::onDisconnected()
{
    m_connected = false;
    // TODO: 设备断开后的清理逻辑
    qDebug() << "[GamepadProcessor] device disconnected";
}

// ═══════════════════════════════════════════════════════════
// 按键处理
// ═══════════════════════════════════════════════════════════

void GamepadProcessor::onButtonA(bool pressed)
{
    // TODO: 实际数据处理（协议解析、指令构造等）
    if (pressed) {
        // 例如：构造发送帧、更新状态机、触发联动逻辑
    }
    emit inputProcessed(QStringLiteral("buttonA"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonB(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonB"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonX(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonX"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonY(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonY"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonL1(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonL1"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonR1(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonR1"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonL3(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonL3"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonR3(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonR3"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonStart(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonStart"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonSelect(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonSelect"), pressed ? 1.0 : 0.0);
}

void GamepadProcessor::onButtonGuide(bool pressed)
{
    emit inputProcessed(QStringLiteral("buttonGuide"), pressed ? 1.0 : 0.0);
}

// ═══════════════════════════════════════════════════════════
// 摇杆处理
// ═══════════════════════════════════════════════════════════

void GamepadProcessor::onAxisLeftX(double value)
{
    // TODO: 摇杆数据处理（死区、缩放、映射等）
    emit inputProcessed(QStringLiteral("axisLeftX"), value);
}

void GamepadProcessor::onAxisLeftY(double value)
{
    emit inputProcessed(QStringLiteral("axisLeftY"), value);
}

void GamepadProcessor::onAxisRightX(double value)
{
    emit inputProcessed(QStringLiteral("axisRightX"), value);
}

void GamepadProcessor::onAxisRightY(double value)
{
    emit inputProcessed(QStringLiteral("axisRightY"), value);
}

// ═══════════════════════════════════════════════════════════
// 扳机处理
// ═══════════════════════════════════════════════════════════

void GamepadProcessor::onTriggerL2(double value)
{
    emit inputProcessed(QStringLiteral("triggerL2"), value);
}

void GamepadProcessor::onTriggerR2(double value)
{
    emit inputProcessed(QStringLiteral("triggerR2"), value);
}
