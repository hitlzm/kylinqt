#ifndef DATARECORDER_H
#define DATARECORDER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QPointer>

// StatusFeedbackHex：转台周期状态帧的应用层结构体（跨线程 QueuedConnection 传参需要完整类型）
#include "../serialport/serialport_turntable_HEX.h"

class VlcVideoItem;   //没有访问成员或调用方法，也没有new/delete，只使用固定大小的指针，前置声明即可，不需要包含对应头文件
class ImageData;      //图像导引头 B 帧解析结果（只存放指针，同理只需前置声明）
class LaserData;      //激光导引头接收帧解析结果

/**
 * @brief 数据保存控制器（方案 C）
 *
 * 一次 startSave() ~ stopSave() 为一个保存会话：
 *   - 图像数据：图像导引头 B 帧中约定的那部分字段，存成 csv（解析后的工程量，不是原始 hex）：
 *     时间(由毫秒+微秒合并、按北京时间给出)/B帧流水号/当前工作通道/俯仰·偏航视线角速度/
 *     光学工作状态/俯仰·偏航框架角/俯仰·偏航陀螺/跟踪状态/跟踪器状态/红外帧编号/电视帧编号
 *   - 激光数据：激光导引头 DYT 状态返回帧中约定的那部分字段，同样存 csv（解析后的工程量）：
 *     时间(毫秒+微秒合并、北京时间)/光轴方位角·俯仰角/方位·俯仰陀螺输出角速度/
 *     方位·俯仰速度环指令输入/方位·俯仰偏差角/激光周期/增益状态/四象限能量强度
 *     （懒创建：某一路第一个帧到达时才建该路文件；数据流暂存，缓冲后落盘）
 *   - 转台数据：转台按固定周期返回的状态反馈帧（解析后的 StatusFeedbackHex），
 *     单独写一个 csv：一行一帧，含转台毫秒时间/指令提示/三轴状态·角度·控制偏差/秒脉冲，
 *     另加会话内自增序号便于核对周期。状态与指令提示存中文含义（不存代号），
 *     表头同样为中文并带 UTF-8 BOM（懒创建，同 200ms 缓冲落盘）
 *   - 视频：通过 VlcVideoItem 的 mpv record-file 录制原始码流（.ts），
 *     停止时调用 ffmpeg 转封装（-c copy）成 MP4，成功则删除临时 TS
 *     若保存过程中视频源被切换/重连（含界面自动重连），录制会就地终止：
 *     不再续录，已录到的部分立即转封装，并发 videoRecordInterrupted() 告知界面
 *
 * 文件路径规则（默认）：
 *   <saveDir>/<日期 M.d>/image<时分秒>.csv      （图像导引头 B 帧解析后字段）
 *   <saveDir>/<日期 M.d>/laser<时分秒>.csv      （激光导引头接收帧解析后字段）
 *   <saveDir>/<日期 M.d>/turntable<时分秒>.csv  （转台周期状态帧）
 *   <saveDir>/<日期 M.d>/video<时分秒>.mp4
 *
 * 运行在主线程（串口数据量约 12KB/s，缓冲写即可），视频转封装用异步 QProcess。
 */
class DataRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool saving READ saving NOTIFY savingChanged)
    Q_PROPERTY(QString saveDir READ saveDir WRITE setSaveDir NOTIFY saveDirChanged)
    Q_PROPERTY(QString ffmpegPath READ ffmpegPath WRITE setFfmpegPath NOTIFY ffmpegPathChanged)

public:
    explicit DataRecorder(QObject *parent = nullptr);
    ~DataRecorder() override;

    // ── 属性 ──
    bool saving() const { return m_saving; }
    QString saveDir() const { return m_saveDir; }
    void setSaveDir(const QString &dir);
    QString ffmpegPath() const { return m_ffmpegPath; }
    void setFfmpegPath(const QString &path);

    // 视频录制对象（main.cpp 在 QML 根对象找到 VlcVideoItem 后注入）
    void setVideoItem(VlcVideoItem *item);
    // 图像导引头解析结果对象（main.cpp 创建后注入）：csv 直接取它的解析值，不再重复换算
    void setImageData(ImageData *data);
    // 激光导引头解析结果对象（同理，csv 取它的解析值）
    void setLaserData(LaserData *data);

public slots:
    // QML 调用入口（后续按钮接这里）
    Q_INVOKABLE bool startSave();
    Q_INVOKABLE void stopSave();

    // 图像 B 帧入口：ImageData 解析完一帧后触发（DirectConnection，读到的就是本帧解析值）
    // 串口未打开或数据校验不过时不会走到这里，所以不用担心保存全 0 值
    void onImageFrame();
    // 激光接收帧入口：LaserData 解析完一帧后触发（DirectConnection，读到的就是本帧解析值）
    void onLaserFrame();

    // 转台周期状态帧入口（工作线程 QueuedConnection 到主线程，每帧一行 csv）
    void onTurntableFrame(const StatusFeedbackHex &frame);

signals:
    void savingChanged();
    void saveDirChanged();
    void ffmpegPathChanged();

    // 保存过程提示（供 QML 弹窗/状态显示）
    void errorOccurred(const QString &msg);
    // 视频转封装结束（ok=false 时保留 .ts 原始文件）
    void videoConvertFinished(bool ok, const QString &mp4Path);
    // 保存过程中视频源被切换/重连，本会话的视频录制已停止（其余数据不受影响）
    void videoRecordInterrupted(const QString &msg);

private:
    bool ensureSaveFolder();
    void ensureImageFile();
    void ensureLaserFile();
    void ensureCsvFile();
    void flushImage();
    void flushLaser();
    void flushCsv();
    void flushAll();
    void startVideoRecord();
    void stopVideoRecord();
    void handleVideoRecordingChanged();   // 视频录制状态变化：中途被换源/重连打断时收尾
    void startRemux();
    // 若目标路径已存在（同秒内重复会话），追加 _1/_2 后缀避免覆盖/混写
    QString makeUniquePath(const QString &basePath) const;

    // ── 配置 ──
    bool m_saving = false;
    QString m_saveDir = "/data/savedata";
    QString m_ffmpegPath = "ffmpeg";    //改成实际的FFMPEG路径
    // 用 QPointer 持有 QML 对象：engine 先于 app 销毁时自动置空，避免悬垂指针
    QPointer<VlcVideoItem> m_videoItem;
    // 图像导引头解析结果（主线程对象，同样用 QPointer 防悬垂）
    QPointer<ImageData> m_imageData;
    // 激光导引头解析结果
    QPointer<LaserData> m_laserData;

    // ── 本次会话（一次 startSave ~ stopSave）──
    QString m_sessionDate;   // 例如 8.21
    QString m_sessionTime;   // 例如 143025
    QString m_sessionDir;    // /home/ipc/savedata/8.21
    QString m_imagePath;            // 图像导引头原始帧
    bool m_imageOpenFailed = false; // 打开失败后不再每帧重试/重复报错
    QString m_laserPath;            // 激光导引头原始帧
    bool m_laserOpenFailed = false;
    QString m_csvPath;              // 转台周期数据
    bool m_csvOpenFailed = false;   // csv 打开失败后不再每帧重试/重复报错
    quint64 m_turntableSeq = 0;     // 本次会话收到的转台帧序号（从 1 开始）
    QString m_videoTsPath;   // 临时 TS，转封装成功后删除
    QString m_videoMp4Path;
    bool m_videoRecordLost = false;   // 本会话视频录制已因换源/重连终止

    // ── 串口文本写缓冲（主线程缓冲，200ms 落盘一次）──
    QFile m_imageFile;
    QByteArray m_imageBuffer;
    QFile m_laserFile;
    QByteArray m_laserBuffer;
    // ── 转台 csv 写缓冲（与上面共用同一个 200ms 落盘定时器）──
    QFile m_csvFile;
    QByteArray m_csvBuffer;
    QTimer m_flushTimer;

    // ── ffmpeg 转封装子进程 ──
    QProcess m_ffmpegProc;
};

#endif // DATARECORDER_H
