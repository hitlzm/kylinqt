#pragma once

#include <QObject>
#include <QTimer>

class Myhandle : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(float axisLeftX  MEMBER m_axisLeftX  NOTIFY axisLeftXChanged)
    // Q_PROPERTY(float axisLeftY  MEMBER m_axisLeftY  NOTIFY axisLeftYChanged)
    // Q_PROPERTY(float axisRightX MEMBER m_axisRightX NOTIFY axisRightXChanged)
    // // Q_PROPERTY(float axisRightY MEMBER m_axisRightY NOTIFY axisRightYChanged)
    // Q_PROPERTY(float buttonL2   MEMBER m_buttonL2   NOTIFY buttonL2Changed)
    // Q_PROPERTY(float buttonR2   MEMBER m_buttonR2   NOTIFY buttonR2Changed)
    // Q_PROPERTY(bool  buttonA    MEMBER m_buttonA    NOTIFY buttonAChanged)
    // Q_PROPERTY(bool  buttonB    MEMBER m_buttonB    NOTIFY buttonBChanged)
private:
    QTimer *m_timer;

public:
    Myhandle( QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    {
        connect(m_timer, &QTimer::timeout, this, [this]{
            emit handleModeSignal(m_axisLeftX, m_axisLeftY, m_axisRightX, m_buttonL2, m_buttonR2, m_buttonA, m_buttonB);
        });
    };
    ~Myhandle(){};
signals:
    // void reqturntablesend();  // 遥控模式时以一定周期通知转台串口线程发送指令

    //切换到手柄模式时发给转台线程的信号
    void handleModeSignal(float axisLeftX, float axisLeftY, float axisRightX, float buttonL2, float buttonR2, bool buttonA, bool buttonB);
public slots:
    //接收QML的通知，看看是否为手柄模式
    void modechanged(int index)
    {   
        //以40ms周期定时发送信号给转台串口线程，通知转台串口线程发送手柄数据
        if(index == 2)
        {
            //启动定时器，每40ms发送一次手柄数据
            m_timer->start(40);
        }
        else{
            //关闭定时器
            m_timer->stop();
        }
    };
    //更新手柄数据的槽函数
    void axisLeftXChanged(float value){ m_axisLeftX = value;};
    void axisLeftYChanged(float value){ m_axisLeftY = value;};
    void axisRightXChanged(float value){ m_axisRightX = value;};
    // void axisRightYChanged(float value){ m_axisRightY = value;};
    void buttonL2Changed(float value){ m_buttonL2 = value;};
    void buttonR2Changed(float value){ m_buttonR2 = value;};
    void buttonAChanged(bool pressed){ m_buttonA = pressed;};
    void buttonBChanged(bool pressed){ m_buttonB = pressed;};
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
