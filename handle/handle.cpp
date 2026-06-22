#include "Handle.h"

Handle::Handle(int deviceId, QObject *parent)
    : QObject(parent)
    , m_gamepad(new QGamepad(deviceId, this))
    , m_deviceId(deviceId)
    , m_isConnected(false)
{
    // 连接核心状态
    connect(m_gamepad, &QGamepad::connectedChanged, 
            this, &Handle::onConnectedChanged);

    // 连接按键（以A键和B键为例，其他同理）
    connect(m_gamepad, &QGamepad::buttonAChanged, 
            this, &Handle::onButtonA);
    connect(m_gamepad, &QGamepad::buttonBChanged, 
            this, &Handle::onButtonB);

    // 连接摇杆轴
    connect(m_gamepad, &QGamepad::axisLeftXChanged, 
            this, &Handle::onAxisLeftX);
    connect(m_gamepad, &QGamepad::axisLeftYChanged, 
            this, &Handle::onAxisLeftY);

    // 初始化时检查一次状态
    onConnectedChanged();
}

Handle::~Handle(){}

void Handle::onConnectedChanged() {
    m_isConnected = m_gamepad->isConnected();
    if (m_isConnected) {
        emit deviceConnected(m_deviceId);
    } else {
        emit deviceDisconnected();
    }
}

void Handle::onButtonA(bool pressed){}
void Handle::onButtonB(bool pressed){}
void Handle::onButtonX(bool pressed){}
void Handle::onButtonY(bool pressed){}
void Handle::onButtonL1(bool pressed){}
void Handle::onButtonR1(bool pressed){}    
void Handle::onButtonL3(bool pressed){}
void Handle::onButtonR3(bool pressed){}
void Handle::onButtonStart(bool pressed){}
void Handle::onButtonSelect(bool pressed){}   
void Handle::onButtonGuide(bool pressed){}  
void Handle::onAxisLeftX(double value){}  
void Handle::onAxisLeftY(double value){} 
void Handle::onAxisRightX(double value){} 
void Handle::onAxisRightY(double value){} 
void Handle::onButtonL2(double value){} 
void Handle::onButtonR2(double value){} 
  

   