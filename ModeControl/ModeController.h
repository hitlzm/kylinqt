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
    Q_PROPERTY(int currentMode READ currentMode NOTIFY currentModeChanged)
    Q_PROPERTY(int exguideSource READ exguideSource NOTIFY exguideSourceChanged)
    Q_PROPERTY(int exguidePeriod READ exguidePeriod NOTIFY exguidePeriodChanged)
    Q_PROPERTY(bool ccdInferenceRequested READ ccdInferenceRequested
               NOTIFY ccdInferenceRequestChanged)
public:
    explicit ModeController(QObject *parent = nullptr) : QObject(parent) {

    }

    ~ModeController() {

    }

    int currentMode() const { return m_currentMode; }
    int exguideSource() const { return m_exguideSource; }
    int exguidePeriod() const { return m_exguidePeriod; }

    // CCD 视觉推理只在外引导模式、CCD 外引导源和跟踪周期都已选定后才允许。
    bool ccdInferenceRequested() const {
        return m_currentMode == 0
                && m_exguideSource == 5
                && m_exguidePeriod > 0;
    }

public slots:
    void setCurrentMode(int mode) {
        if (m_currentMode == mode)
            return;

        const bool wasRequested = ccdInferenceRequested();
        const bool leavingExguide = (m_currentMode == 0 && mode != 0);
        if (leavingExguide) {
            clearExguideSelection();
            emit exguideSessionEnded();
        }

        m_currentMode = mode;
        emit currentModeChanged();
        emit modeChanged(mode);
        notifyInferenceRequestIfChanged(wasRequested);
    }

    void setExguideSource(int source) {
        // 3: 图像导引头，4: 激光导引头，5: CCD 相机。
        if (source != 0 && source != 3 && source != 4 && source != 5)
            return;
        if (m_exguideSource == source)
            return;

        const bool wasRequested = ccdInferenceRequested();
        m_exguideSource = source;
        emit exguideSourceChanged();
        emit exguideSrcChanged(source);
        notifyInferenceRequestIfChanged(wasRequested);
    }

    void setExguidePeriod(int setting) {
        // 6: 20ms，7: 1s；-1 表示尚未选择跟踪周期。
        if (setting != -1 && setting != 6 && setting != 7)
            return;
        if (m_exguidePeriod == setting)
            return;

        const bool wasRequested = ccdInferenceRequested();
        m_exguidePeriod = setting;
        emit exguidePeriodChanged();
        emit exguideSettingChanged(setting);
        notifyInferenceRequestIfChanged(wasRequested);
    }

    // 图像导引头作为外引导源时帧编号停止更新（通信中断）：
    // 保留当前外引导模式，只通知界面取消「图像导引头」外引导源选择。
    void onExguideLinkLost() {
        clearExguideSourceIf(3);
        emit exguideSourceLost(3);
    }

    // 激光导引头串口未打开：保留外引导模式，只取消激光导引头选择。
    void onExguideLaserSourceLost() {
        clearExguideSourceIf(4);
        emit exguideSourceLost(4);
    }

    // CCD 视频流未接入：保留外引导模式，只取消 CCD 相机选择。
    void onExguideCcdSourceLost() {
        clearExguideSourceIf(5);
        emit exguideSourceLost(5);
    }

signals:
    void currentModeChanged();
    void modeChanged(int newMode); // 新增信号，通知模式已改变，数值取0，1，2
    void exguideSourceChanged();
    void exguideSrcChanged(int exsrc);  //通知外引导源变化， 数值取3，4，5
    void exguidePeriodChanged();
    void exguideSettingChanged(int setting); //通知外引导发送时间间隔变化 6,7
    void exguideSourceLost(int source);  //外引导源不可用，通知界面只取消对应源
    void exguideSessionEnded();          //退出外引导模式，外部据此清理跟踪显示
    void ccdInferenceRequestChanged(bool enabled);
private slots:
    

private:
    void notifyInferenceRequestIfChanged(bool wasRequested) {
        const bool requested = ccdInferenceRequested();
        if (requested != wasRequested)
            emit ccdInferenceRequestChanged(requested);
    }

    void clearExguideSourceIf(int source) {
        if (m_exguideSource != source)
            return;
        const bool wasRequested = ccdInferenceRequested();
        m_exguideSource = 0;
        emit exguideSourceChanged();
        notifyInferenceRequestIfChanged(wasRequested);
    }

    // 退出外引导后必须重新选择来源和跟踪周期；这里只更新主线程状态，
    // 串口工作线程会在随后收到 modeChanged 时停止并清除各自的外引导绑定。
    void clearExguideSelection() {
        const bool sourceChanged = (m_exguideSource != 0);
        const bool periodChanged = (m_exguidePeriod != -1);

        m_exguideSource = 0;
        m_exguidePeriod = -1;

        if (sourceChanged)
            emit exguideSourceChanged();
        if (periodChanged)
            emit exguidePeriodChanged();
    }

    int m_currentMode = -1;
    int m_exguideSource = -1;
    int m_exguidePeriod = -1;
};

#endif // MODECONTROLLER_H
