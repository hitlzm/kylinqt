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
 * 三路数据（图像 / 激光 / 转台）各自独立启停，互不影响：任意一路停止，其余路继续保存。
 * 三路都默认带视频，视频用“保存请求计数”管理：第一个请求开始时启动录制，计数归零
 * （最后一路停止）时才真正停录并转封装 —— 所以多个请求共用同一个 mp4 文件。
 *
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
 *     三路都默认带视频：第一个保存请求开始录制，计数归零时才停录并转封装，
 *     所以多路同时保存只产生一份 mp4；没有视频播放器/始终没画面时不会产生视频文件
 *     若保存过程中视频源被切换/重连（含界面自动重连），录制会就地终止：
 *     不再续录，已录到的部分立即转封装，并发 videoRecordInterrupted() 告知界面
 *
 * 文件路径规则（默认）：
 *   <saveDir>/<日期 M.d>/image<时分秒>.csv      （时分秒 = 图像这一路开始保存的时刻）
 *   <saveDir>/<日期 M.d>/laser<时分秒>.csv      （时分秒 = 激光这一路开始保存的时刻）
 *   <saveDir>/<日期 M.d>/turntable<时分秒>.csv  （时分秒 = 转台这一路开始保存的时刻）
 *   <saveDir>/<日期 M.d>/video<时分秒>.mp4      （时分秒 = 第一个保存请求的时刻）
 *
 * 运行在主线程（串口数据量约 12KB/s，缓冲写即可），视频转封装用异步 QProcess。
 */
class DataRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool saving READ saving NOTIFY savingChanged)                       // 任一路在保存
    Q_PROPERTY(bool savingImage READ savingImage NOTIFY savingImageChanged)
    Q_PROPERTY(bool savingLaser READ savingLaser NOTIFY savingLaserChanged)
    Q_PROPERTY(bool savingTurntable READ savingTurntable NOTIFY savingTurntableChanged)
    Q_PROPERTY(QString saveDir READ saveDir WRITE setSaveDir NOTIFY saveDirChanged)
    Q_PROPERTY(QString ffmpegPath READ ffmpegPath WRITE setFfmpegPath NOTIFY ffmpegPathChanged)

public:
    explicit DataRecorder(QObject *parent = nullptr);
    ~DataRecorder() override;

    // ── 属性 ──
    bool saving() const { return anySaving(); }
    bool savingImage() const { return m_image.saving; }
    bool savingLaser() const { return m_laser.saving; }
    bool savingTurntable() const { return m_turntable.saving; }
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
    // QML 调用入口：三路各自启停（都默认带视频，视频按请求计数共用一个 mp4）
    Q_INVOKABLE bool startImageSave();
    Q_INVOKABLE void stopImageSave();
    Q_INVOKABLE bool startLaserSave();
    Q_INVOKABLE void stopLaserSave();
    Q_INVOKABLE bool startTurntableSave();
    Q_INVOKABLE void stopTurntableSave();
    // 退出前一次性收尾：等价于三路依次停止（最后一路会触发视频转封装）
    Q_INVOKABLE void stopAll();

    // 图像 B 帧入口：ImageData 解析完一帧后触发（DirectConnection，读到的就是本帧解析值）
    // 串口未打开或数据校验不过时不会走到这里，所以不用担心保存全 0 值
    void onImageFrame();
    // 激光接收帧入口：LaserData 解析完一帧后触发（DirectConnection，读到的就是本帧解析值）
    void onLaserFrame();

    // 转台周期状态帧入口（工作线程 QueuedConnection 到主线程，每帧一行 csv）
    void onTurntableFrame(const StatusFeedbackHex &frame);

signals:
    void savingChanged();
    void savingImageChanged();
    void savingLaserChanged();
    void savingTurntableChanged();
    void saveDirChanged();
    void ffmpegPathChanged();

    // 保存过程提示（供 QML 弹窗/状态显示）
    void errorOccurred(const QString &msg);
    // 视频转封装结束（ok=false 时保留 .ts 原始文件）
    void videoConvertFinished(bool ok, const QString &mp4Path);
    // 保存过程中视频源被切换/重连，本会话的视频录制已停止（其余数据不受影响）
    void videoRecordInterrupted(const QString &msg);

private:
    // 一路数据的保存状态（图像 / 激光 / 转台 各一份）
    struct SourceState {
        bool saving = false;          // 该路是否正在保存
        QString dir;                  // <saveDir>/<日期>，本次该路用的目录
        QString time;                 // 本次该路开始保存的时分秒
        QString path;                 // 该路数据文件全路径
        bool openFailed = false;      // 打开失败后不再每帧重试/重复报错
        QFile file;
        QByteArray buffer;            // 主线程缓冲，200ms 落盘一次
    };

    // ── 启停 ──
    bool startSource(SourceState &st, const char *name);
    bool stopSource(SourceState &st, const char *name);
    bool anySaving() const { return m_image.saving || m_laser.saving || m_turntable.saving; }
    void updateFlushTimer();

    // ── 视频（按保存请求计数）──
    void acquireVideoRecording(const QString &dir, const QString &time);
    void releaseVideoRecording();
    void startVideoRecord(const QString &dir, const QString &time);
    void stopVideoRecord();
    void handleVideoRecordingChanged();   // 视频录制状态变化：中途被换源/重连打断时收尾
    void startRemux();

    // ── 文件与缓冲 ──
    void ensureSourceFile(SourceState &st, const QString &prefix,
                          const QByteArray &header, const char *name);
    void flushSource(SourceState &st, const char *name);
    void flushAll();
    // 若目标路径已存在（同秒内重复会话），追加 _1/_2 后缀避免覆盖/混写
    QString makeUniquePath(const QString &basePath) const;

    // ── 配置 ──
    QString m_saveDir = "/data/savedata";
    QString m_ffmpegPath = "ffmpeg";    //改成实际的FFMPEG路径
    // 用 QPointer 持有 QML 对象：engine 先于 app 销毁时自动置空，避免悬垂指针
    QPointer<VlcVideoItem> m_videoItem;
    // 图像导引头解析结果（主线程对象，同样用 QPointer 防悬垂）
    QPointer<ImageData> m_imageData;
    // 激光导引头解析结果
    QPointer<LaserData> m_laserData;

    // ── 三路各自的状态（各自目录/文件/缓冲/时间戳）──
    SourceState m_image;        // 图像导引头 B 帧
    SourceState m_laser;        // 激光导引头接收帧
    SourceState m_turntable;    // 转台周期状态帧
    quint64 m_turntableSeq = 0; // 本次转台保存收到的帧序号（从 1 开始）

    // ── 视频：多个保存请求共用一份录像 ──
    int m_videoRefCount = 0;         // 保存请求计数，归零时才真正停录
    QString m_videoTsPath;   // 临时 TS，转封装成功后删除
    QString m_videoMp4Path;
    bool m_videoRecordLost = false;  // 本次视频录制已因换源/重连终止（不再续录）

    QTimer m_flushTimer;   // 任一路在保存时运行，统一 200ms 落盘

    // ── ffmpeg 转封装子进程 ──
    QProcess m_ffmpegProc;
};

#endif // DATARECORDER_H
