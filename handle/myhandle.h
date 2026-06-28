#pragma once

#include <QObject>

class Myhandle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float axisLeftX  MEMBER m_axisLeftX  NOTIFY axisLeftXChanged)
    Q_PROPERTY(float axisLeftY  MEMBER m_axisLeftY  NOTIFY axisLeftYChanged)
    Q_PROPERTY(float axisRightX MEMBER m_axisRightX NOTIFY axisRightXChanged)
    // Q_PROPERTY(float axisRightY MEMBER m_axisRightY NOTIFY axisRightYChanged)
    Q_PROPERTY(float buttonL2   MEMBER m_buttonL2   NOTIFY buttonL2Changed)
    Q_PROPERTY(float buttonR2   MEMBER m_buttonR2   NOTIFY buttonR2Changed)
    Q_PROPERTY(bool  buttonA    MEMBER m_buttonA    NOTIFY buttonAChanged)
    Q_PROPERTY(bool  buttonB    MEMBER m_buttonB    NOTIFY buttonBChanged)

public:
    Myhandle( QObject *parent)
    : QObject(parent)
    {  
    };
    ~Myhandle(){};
signals:
    void reqturntablesend();  // 遥控模式时以一定周期通知转台串口线程发送指令

    void axisLeftXChanged(float value);
    void axisLeftYChanged(float value);
    void axisRightXChanged(float value);
    void axisRightYChanged(float value);
    void buttonL2Changed(float value);
    void buttonR2Changed(float value);
    void buttonAChanged(bool pressed);
    void buttonBChanged(bool pressed);

private:
    float m_axisLeftX  = 0.0f;
    float m_axisLeftY  = 0.0f;
    float m_axisRightX = 0.0f;
    // float m_axisRightY = 0.0f;
    float m_buttonL2   = 0.0f;
    float m_buttonR2   = 0.0f;
    bool  m_buttonA    = false;
    bool  m_buttonB    = false;
};
