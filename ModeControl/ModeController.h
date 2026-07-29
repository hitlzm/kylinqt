// modecontroller.h
#ifndef MODECONTROLLER_H
#define MODECONTROLLER_H

#include <QObject>
#include <QThread>


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
        
    }

    ~ModeController() {
       
    }

signals:
    void modeChanged(int newMode); // 新增信号，通知模式已改变，数值取0，1，2
    void exguideSrcChanged(int exsrc);  //通知外引导源变化， 数值取3，4，5
    void exguideSettingChanged(int setting); //通知外引导发送时间间隔变化 6,7
private slots:
    

private:
    
};

#endif // MODECONTROLLER_H