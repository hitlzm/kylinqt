#ifndef DATARECORDER_H
#define DATARECORDER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QPointer>

class VlcVideoItem;   //没有访问成员或调用方法，也灭没有new/delete，只使用固定大小的指针，前置声明即可，不需要包含对应头文件

/**
 * @brief 数据保存控制器（方案 C）
 *
 * 一次 startSave() ~ stopSave() 为一个保存会话：
 *   - 串口数据：图像/激光导引头收到的原始帧字节，谁收到就追加到同一个 txt 文件
 *     （懒创建：第一个帧到达时才建文件；原始数据流暂存，缓冲后落盘）
 *   - 视频：通过 VlcVideoItem 的 mpv record-file 录制原始码流（.ts），
 *     停止时调用 ffmpeg 转封装（-c copy）成 MP4，成功则删除临时 TS
 *
 * 文件路径规则（默认）：
 *   /home/ipc/savedata/<日期 M.d>/txt<时分秒>.txt
 *   /home/ipc/savedata/<日期 M.d>/video<时分秒>.mp4
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

public slots:
    // QML 调用入口（后续按钮接这里）
    Q_INVOKABLE bool startSave();
    Q_INVOKABLE void stopSave();

    // 串口原始帧入口（工作线程 QueuedConnection 到主线程）
    void onImageFrame(const QByteArray &frame);
    void onLaserFrame(const QByteArray &frame);

signals:
    void savingChanged();
    void saveDirChanged();
    void ffmpegPathChanged();

    // 保存过程提示（供 QML 弹窗/状态显示）
    void errorOccurred(const QString &msg);
    // 视频转封装结束（ok=false 时保留 .ts 原始文件）
    void videoConvertFinished(bool ok, const QString &mp4Path);

private:
    bool ensureSaveFolder();
    void ensureTxtFile();
    void flushTxt();
    void startVideoRecord();
    void stopVideoRecord();
    void startRemux();
    // 若目标路径已存在（同秒内重复会话），追加 _1/_2 后缀避免覆盖/混写
    QString makeUniquePath(const QString &basePath) const;

    // ── 配置 ──
    bool m_saving = false;
    QString m_saveDir = "/home/ipc/savedata";
    QString m_ffmpegPath = "ffmpeg";    //改成实际的FFMPEG路径
    // 用 QPointer 持有 QML 对象：engine 先于 app 销毁时自动置空，避免悬垂指针
    QPointer<VlcVideoItem> m_videoItem;

    // ── 本次会话（一次 startSave ~ stopSave）──
    QString m_sessionDate;   // 例如 8.21
    QString m_sessionTime;   // 例如 143025
    QString m_sessionDir;    // /home/ipc/savedata/8.21
    QString m_txtPath;
    bool m_txtOpenFailed = false;   // txt 打开失败后不再每帧重试/重复报错
    QString m_videoTsPath;   // 临时 TS，转封装成功后删除
    QString m_videoMp4Path;

    // ── 串口 txt 写缓冲（主线程缓冲，200ms 落盘一次）──
    QFile m_txtFile;
    QByteArray m_txtBuffer;
    QTimer m_flushTimer;

    // ── ffmpeg 转封装子进程 ──
    QProcess m_ffmpegProc;
};

#endif // DATARECORDER_H
