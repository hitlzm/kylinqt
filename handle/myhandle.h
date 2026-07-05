#pragma once

#include <QObject>
#include <QTimer>
#include <QMutex>
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
            // 加锁读取 → 拷贝到信号参数 → 解锁，信号参数是值拷贝，线程安全
            QMutexLocker locker(&m_mutex);
            emit handleModeSignal(m_axisLeftX, m_axisLeftY, m_axisRightX,
                                  m_buttonL2, m_buttonR2, m_buttonA, m_buttonB);
        });
    }

    ~Myhandle() override = default;

signals:
    void handleModeSignal(float axisLeftX, float axisLeftY, float axisRightX,
                          float buttonL2, float buttonR2, bool buttonA, bool buttonB);

public slots:
    void modechanged(int index)
    {
        if (index == 2) {
            m_timer->start(40);
        } else {
            m_timer->stop();
        }
    }

    // ── 手柄数据更新槽（主线程 QML 调用 → 加锁写入）──
    void axisLeftXChanged(float value)  { QMutexLocker locker(&m_mutex); m_axisLeftX  = value; 
    // qDebug()   经测试发现，直接在QML中调用槽函数还是在主线程运行，QML接收手柄信号易造成主界面卡顿
    // << "Current:"
    // << QThread::currentThread();
    // qDebug()
    // << "Object:"
    // << thread();
    }
    void axisLeftYChanged(float value)  { QMutexLocker locker(&m_mutex); m_axisLeftY  = value; }
    void axisRightXChanged(float value) { QMutexLocker locker(&m_mutex); m_axisRightX = value; }
    void buttonL2Changed(float value)   { QMutexLocker locker(&m_mutex); m_buttonL2   = value; }
    void buttonR2Changed(float value)   { QMutexLocker locker(&m_mutex); m_buttonR2   = value; }
    void buttonAChanged(bool pressed)   { QMutexLocker locker(&m_mutex); m_buttonA    = pressed; }
    void buttonBChanged(bool pressed)   { QMutexLocker locker(&m_mutex); m_buttonB    = pressed; }

private:
    QTimer *m_timer;
    mutable QMutex m_mutex;

    float m_axisLeftX  = 0.0f;
    float m_axisLeftY  = 0.0f;
    float m_axisRightX = 0.0f;
    float m_buttonL2   = 0.0f;
    float m_buttonR2   = 0.0f;
    bool  m_buttonA    = false;
    bool  m_buttonB    = false;
};
