#pragma once

#include <QObject>
#include <QTimer>
#include <QDebug>

class Myhandle : public QObject
{
    Q_OBJECT

public:
    Myhandle(QObject *parent)
        : QObject(parent)
        , m_timer(new QTimer(this))
    {
        connect(m_timer, &QTimer::timeout, this, [this]{
            emit handleModeSignal(m_axisLeftX, m_axisLeftY, m_axisRightX,
                                  m_buttonL2, m_buttonR2, m_buttonA, m_buttonB);
        });
    }

    ~Myhandle() override { delete m_timer; }

signals:
    void handleModeSignal(float axisLeftX, float axisLeftY, float axisRightX,
                          float buttonL2, float buttonR2, bool buttonA, bool buttonB);

public slots:
    void modechanged(int index)
    {
        if (index == 2) {
            m_timer->start(50);
        } else {
            m_timer->stop();
        }
    }

    // 手柄数据更新槽 —— 全部由 GamepadBridge 信号经 QueuedConnection 在 Handlethread 调用
    void axisLeftXChanged(float value)  { m_axisLeftX  = value; }
    void axisLeftYChanged(float value)  { m_axisLeftY  = value; }
    void axisRightXChanged(float value) { m_axisRightX = value; }
    void buttonL2Changed(float value)   { m_buttonL2   = value; }
    void buttonR2Changed(float value)   { m_buttonR2   = value; }
    void buttonAChanged(bool pressed)   { m_buttonA    = pressed; }
    void buttonBChanged(bool pressed)   { m_buttonB    = pressed; }

    void update(
                    float axisLeftX,
                    float axisLeftY,
                    float axisRightX,
                    bool buttonA,
                    bool buttonB,
                    float buttonL2,
                    float buttonR2)
            {
                m_axisLeftX = axisLeftX;
                m_axisLeftY = axisLeftY;
                m_axisRightX = axisRightX;
                m_buttonL2 = buttonL2;
                m_buttonR2 = buttonR2;
                m_buttonA = buttonA;
                m_buttonB = buttonB;
            }

private:
    QTimer *m_timer;

    float m_axisLeftX  = 0.0f;
    float m_axisLeftY  = 0.0f;
    float m_axisRightX = 0.0f;
    float m_buttonL2   = 0.0f;
    float m_buttonR2   = 0.0f;
    bool  m_buttonA    = false;
    bool  m_buttonB    = false;
};

class GamepadBridge : public QObject
{
    Q_OBJECT

public:
    explicit GamepadBridge(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void axisLeftXChange(float value);
    void axisLeftYChange(float value);
    void axisRightXChange(float value);
    void buttonL2Change(float value);
    void buttonR2Change(float value);
    void buttonAChange(bool pressed);
    void buttonBChange(bool pressed);
    void updateGamepad(
                    float axisLeftX,
                    float axisLeftY,
                    float axisRightX,
                    bool buttonA,
                    bool buttonB,
                    float buttonL2,
                    float buttonR2);
};
