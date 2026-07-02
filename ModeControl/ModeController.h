// modecontroller.h
#ifndef MODECONTROLLER_H
#define MODECONTROLLER_H

#include <QObject>
#include <QThread>
#include "worker.h"

/*
在这里区分是对转台的哪种控制模式，将模式切换信号绑定到多个槽函数上
如果是遥控模式，发送信号给手柄类，连接的槽函数中会定时发送手柄数据给转台串口线程并发送  //手柄模式区分速度模式and角度模式
如果是程控模式，通知转台串口线程已切换到程控模式
如果是外引导模式，通知导引头线程同步数据给转台串口线程并发送
*/
class ModeController : public QObject
{
    Q_OBJECT
public:
    explicit ModeController(QObject *parent = nullptr) : QObject(parent) {
        // 1. 创建工作对象
        Worker *worker = new Worker();
        // 2. 创建线程
        QThread *thread = new QThread(this);
        // 3. 将工作对象移动到子线程
        worker->moveToThread(thread);
        
        // 4. 连接信号与槽
        // 当QML调用startWork()时，会触发此信号，进而调用Worker::startWork()（在子线程执行）
        connect(this, &ModeController::startWork, worker, &Worker::startWork);
        // 当Worker发出progressUpdated，会触发ModeController::onProgressUpdated（在主线程执行）
        connect(worker, &Worker::progressUpdated, this, &ModeController::onProgressUpdated);
        // 当Worker发出workFinished，会触发ModeController::onWorkFinished（在主线程执行）
        connect(worker, &Worker::workFinished, this, &ModeController::onWorkFinished);

        // 5. 启动线程
        thread->start();

        // 保存worker和thread指针以便后续清理
        m_worker = worker;
        m_thread = thread;
    }

    ~ModeController() {
        // 安全退出线程
        m_thread->quit();
        m_thread->wait();
        // 注意：worker 会在 m_thread 中被销毁，或者可以手动销毁
    }

signals:
    // QML 通过调用此信号来启动任务
    void startWork();
    // 这些信号会转发 Worker 的结果给 QML
    void progressUpdated(int value);
    void workFinished(const QString &result);

private slots:
    // 这些槽在主线程执行，用于接收Worker的信号并转发
    void onProgressUpdated(int value) {
        emit progressUpdated(value); // 转发给QML
    }
    void onWorkFinished(const QString &result) {
        emit workFinished(result);   // 转发给QML
    }

private:
    Worker *m_worker;
    QThread *m_thread;
};

#endif // MODECONTROLLER_H