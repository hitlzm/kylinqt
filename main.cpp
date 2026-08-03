#include <QGuiApplication>
#include <QApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QFont>
#include "serialport/serialport_laser.h"
#include "serialport/serialport_image.h"
#include "serialport/serialport_turntable_HEX.h"
#include "serialport/serialport_BD.h"
#include "serialport/serialport_CCD.h"
#include "vlcvideo/VlcVideoItem.h"
#include "vlcvideo/VlcFrameItem.h"
#include "handle/myhandle.h"
#include "ModeControl/ModeController.h"
#include "log/LogManager.h"
#include "network/TemplateBindingClient.h"

//使用GPU来做图像绘制
#ifdef _WIN32
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
#endif

int main(int argc, char *argv[])
{
    // ── DPI 缩放：Windows 依赖系统缩放比例，Linux 可通过环境变量 QT_SCALE_FACTOR 覆盖 ──
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("KylinQt");
    QCoreApplication::setOrganizationDomain("kylinqt.local");
    QCoreApplication::setApplicationName("kylin-qt");
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    // Qt 5.14+ 提供更精确的高 DPI 处理策略
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    // QGuiApplication app(argc, argv);
    
    QApplication app(argc, argv);

    // 固定 QQC2 样式为 Basic，避免在 Linux/麒麟上因 QApplication 自动切到 Fusion
    // 导致 TabBar 黑白选中效果丢失、整体长度缩小（Windows 下默认即 Basic）
    QQuickStyle::setStyle("Basic");

    // ── 跨平台默认字体：Linux 上不存在微软雅黑，设置可用回退字体 ──
    QFont defaultFont = app.font();
#ifdef _WIN32
    defaultFont.setFamily("Microsoft YaHei");
#else
    // 银河麒麟 V10 常见中文字体
    defaultFont.setFamily("Noto Sans CJK SC");
    defaultFont.setPixelSize(defaultFont.pixelSize());
#endif
    app.setFont(defaultFont);

    // 注册自定义结构体到 Qt 元对象系统（QueuedConnection 跨线程传递必需）
    qRegisterMetaType<laser_send_frame>("laser_send_frame");
    qRegisterMetaType<laser_recv_frame>("laser_recv_frame");
    qRegisterMetaType<image_send_frame>("image_send_frame");
    qRegisterMetaType<image_recv_frame>("image_recv_frame");
    qRegisterMetaType<programSend_frameHex>("programSend_frameHex");
    qRegisterMetaType<StatusFeedbackHex>("StatusFeedbackHex");
    qRegisterMetaType<RMCData>("RMCData");
    qRegisterMetaType<sendExGuideData>("sendExGuideData");

    // ═══ 主线程对象：QML 直接访问 ═══
    LaserData *laserData = new LaserData(&app);
    LaserSendData *laserSendData = new LaserSendData(&app);
    ImageData *imageData = new ImageData(&app);
    ImageSendData *imageSendData = new ImageSendData(&app);
    TurntableDataHex * turntableData = new TurntableDataHex(&app);
    TurntableSendDataHex *turntableSendData = new TurntableSendDataHex(&app);
    BDData *bdData = new BDData(&app);
    CCDData *ccdData = new CCDData(&app);
    //创建模板装订数据对象（主线程，QML 直接访问）
    TemplateBindingData *templateBindingData = new TemplateBindingData(&app);

    // 全局单例日志管理器
    LogManager *logManager = LogManager::instance();
    logManager->setParent(&app);

    // ═══ 工作线程对象：只处理串口 I/O ═══
    SerialPortLaser *laserPort = new SerialPortLaser;       // 无父对象
    SerialPortImage *imagePort = new SerialPortImage;
    SerialPortTurntableHex *turntablePort = new SerialPortTurntableHex;
    SerialPortBD *bdPort = new SerialPortBD;
    SerialPortCCD *ccdPort = new SerialPortCCD;       // 无父对象，将移到子线程
    // 网络传输 Worker（移到 NetworkThread）
    TemplateBindingWorker *networkWorker = new TemplateBindingWorker(templateBindingData);

    // 把 Data 对象挂给 Worker 存引用（parseData 需要 m_laserData->updateFromFrame）
    laserPort->m_laserData = laserData;
    laserPort->m_laserSendData = laserSendData;
    imagePort->m_imageData = imageData;
    imagePort->m_imageSendData = imageSendData;
    turntablePort->m_turntableDataHex = turntableData;
    turntablePort->m_turntableSendDataHex = turntableSendData;
    ccdPort->m_ccdData = ccdData;
    //创建手柄对象
    Myhandle *_myhandle = new Myhandle(nullptr);   // 无父对象，将移到子线程
    //创建模式管理对象
    ModeController m_modeController(&app);  //释放的信号分别连接到手柄线程和导引头串口线程
    GamepadBridge *m_gamepadBridge = new GamepadBridge(&app);


    // ═══ 1) 先加载 QML，建立绑定 ═══
    QQmlApplicationEngine engine;
    engine.addImportPath(TaoQuickImportPath);
    engine.addImportPath(app.applicationDirPath());
    engine.rootContext()->setContextProperty("taoQuickImportPath", TaoQuickImportPath);
    engine.rootContext()->setContextProperty("laserData", laserData);
    engine.rootContext()->setContextProperty("laserSendData", laserSendData);
    engine.rootContext()->setContextProperty("imageData", imageData);
    engine.rootContext()->setContextProperty("imageSendData", imageSendData);
    engine.rootContext()->setContextProperty("turntableData", turntableData);
    engine.rootContext()->setContextProperty("turntableSendData", turntableSendData);
    engine.rootContext()->setContextProperty("bdData", bdData);
    engine.rootContext()->setContextProperty("ccdData", ccdData);
    // engine.rootContext()->setContextProperty("handle", _myhandle);
    engine.rootContext()->setContextProperty("modeController", &m_modeController);
    engine.rootContext()->setContextProperty("gamepadBridge", m_gamepadBridge);
    engine.rootContext()->setContextProperty("templateBindingData", templateBindingData);
    engine.rootContext()->setContextProperty("logManager", logManager);
    qmlRegisterType<VlcVideoItem>("VlcVideo", 1, 0, "VlcVideo");
    qmlRegisterType<VlcFrameItem>("VlcVideo", 1, 0, "VlcFrame");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qCritical() << "Failed to load QML file";
            QCoreApplication::exit(-1);
        }
    }, Qt::DirectConnection);
    engine.load(url);

    // ═══ 2) 连线：Data（主线程）↔ Worker（工作线程），全部 QueuedConnection ═══

    // ── Laser: 主线程 Data → 工作线程 Worker ──
    QObject::connect(laserData, &LaserData::requestOpenPort,  laserPort, &SerialPortLaser::onOpenPort,  Qt::QueuedConnection);
    QObject::connect(laserData, &LaserData::requestClosePort, laserPort, &SerialPortLaser::onClosePort, Qt::QueuedConnection);
    QObject::connect(laserData, &LaserData::requestScanPorts, laserPort, &SerialPortLaser::onScanPorts, Qt::QueuedConnection);
    QObject::connect(laserSendData, &LaserSendData::requestSendData,  laserPort, &SerialPortLaser::onSendData,  Qt::QueuedConnection);

    // ── Laser: 工作线程 Worker → 主线程 Data ──
    QObject::connect(laserPort, &SerialPortLaser::portOpened,   laserData, &LaserData::setPortOpen, Qt::QueuedConnection);
    QObject::connect(laserPort, &SerialPortLaser::portClosed,   laserData, [laserData]{ laserData->setPortOpen(false); }, Qt::QueuedConnection);
    QObject::connect(laserPort, &SerialPortLaser::portError,    laserData, &LaserData::setError,    Qt::QueuedConnection);
    QObject::connect(laserPort, &SerialPortLaser::portsChanged, laserData, &LaserData::setPortList, Qt::QueuedConnection);
    QObject::connect(laserPort, &SerialPortLaser::laserFrameReceived, laserData, &LaserData::updateFromFrame, Qt::QueuedConnection);
    
    // ── Image: 主线程 Data → 工作线程 Worker ──
    QObject::connect(imageData, &ImageData::requestOpenPort,  imagePort, &SerialPortImage::onOpenPort,  Qt::QueuedConnection);
    QObject::connect(imageData, &ImageData::requestClosePort, imagePort, &SerialPortImage::onClosePort, Qt::QueuedConnection);
    QObject::connect(imageData, &ImageData::requestScanPorts, imagePort, &SerialPortImage::onScanPorts, Qt::QueuedConnection);
    QObject::connect(imageSendData, &ImageSendData::requestSendData,  imagePort, &SerialPortImage::onSendData,  Qt::QueuedConnection);


    // ── Image: 工作线程 Worker → 主线程 Data ──
    QObject::connect(imagePort, &SerialPortImage::portOpened,   imageData, &ImageData::setPortOpen, Qt::QueuedConnection);
    QObject::connect(imagePort, &SerialPortImage::portClosed,   imageData, [imageData]{ imageData->setPortOpen(false); }, Qt::QueuedConnection);
    QObject::connect(imagePort, &SerialPortImage::portError,    imageData, &ImageData::setError,    Qt::QueuedConnection);
    QObject::connect(imagePort, &SerialPortImage::portsChanged, imageData, &ImageData::setPortList, Qt::QueuedConnection);
    QObject::connect(imagePort, &SerialPortImage::imageFrameReceived, imageData, &ImageData::updateFromFrame, Qt::QueuedConnection);

    // ── 偏差像素链：QML点击 → VlcVideoItem → imageSendData(桥) → imagePort → imageSendData ──
    QObject::connect(imageSendData, &ImageSendData::deviationPixelRelayed, imagePort, &SerialPortImage::recvDeviationPixel, Qt::QueuedConnection);
    QObject::connect(imagePort, &SerialPortImage::reqSendDeviationPixel, imageSendData, &ImageSendData::buildDeviation, Qt::QueuedConnection);

    // ── Turntable: 主线程 Data → 工作线程 Worker ──
    QObject::connect(turntableData, &TurntableDataHex::requestOpenPort,  turntablePort, &SerialPortTurntableHex::onOpenPort,  Qt::QueuedConnection);
    QObject::connect(turntableData, &TurntableDataHex::requestClosePort, turntablePort, &SerialPortTurntableHex::onClosePort, Qt::QueuedConnection);
    QObject::connect(turntableData, &TurntableDataHex::requestScanPorts, turntablePort, &SerialPortTurntableHex::onScanPorts, Qt::QueuedConnection);

    // ── Turntable: 工作线程 Worker → 主线程 Data ──
    QObject::connect(turntablePort, &SerialPortTurntableHex::portOpened,   turntableData, &TurntableDataHex::setPortOpen, Qt::QueuedConnection);
    QObject::connect(turntablePort, &SerialPortTurntableHex::portClosed,   turntableData, [turntableData]{ turntableData->setPortOpen(false); }, Qt::QueuedConnection);
    QObject::connect(turntablePort, &SerialPortTurntableHex::portError,    turntableData, &TurntableDataHex::setError,    Qt::QueuedConnection);
    QObject::connect(turntablePort, &SerialPortTurntableHex::portsChanged, turntableData, &TurntableDataHex::setPortList, Qt::QueuedConnection);

    //Turntable 信号与槽连接
    QObject::connect(turntableSendData, &TurntableSendDataHex::requestSendProgramMode,   turntablePort, &SerialPortTurntableHex::sendProgramMode, Qt::QueuedConnection);
    QObject::connect(turntableSendData, &TurntableSendDataHex::reqopenTurntable,   turntablePort, &SerialPortTurntableHex::openTurntable, Qt::QueuedConnection);
    QObject::connect(turntableSendData, &TurntableSendDataHex::reqzeroTurntable,   turntablePort, &SerialPortTurntableHex::zeroTurntable, Qt::QueuedConnection);
    QObject::connect(turntableSendData, &TurntableSendDataHex::reqresetTurntable,   turntablePort, &SerialPortTurntableHex::resetTurntable, Qt::QueuedConnection);
    QObject::connect(turntableSendData, &TurntableSendDataHex::reqcloseTurntable,   turntablePort, &SerialPortTurntableHex::closeTurntable, Qt::QueuedConnection);

    QObject::connect(turntablePort, &SerialPortTurntableHex::requpdateframe,   turntableData, &TurntableDataHex::updateframe, Qt::QueuedConnection);
    QObject::connect(turntableData, &TurntableDataHex::myinner_angleChanged,  turntableSendData, &TurntableSendDataHex::recvinner_angle, Qt::QueuedConnection);
    QObject::connect(turntableData, &TurntableDataHex::myinner_angleChanged,  turntablePort, &SerialPortTurntableHex::recvinner_angle, Qt::QueuedConnection);
    QObject::connect(turntableData, &TurntableDataHex::mymiddle_angleChanged,  turntableSendData, &TurntableSendDataHex::recvmiddle_angle, Qt::QueuedConnection);
    QObject::connect(turntableData, &TurntableDataHex::mymiddle_angleChanged,  turntablePort, &SerialPortTurntableHex::recvmiddle_angle, Qt::QueuedConnection);
    QObject::connect(turntableData, &TurntableDataHex::myoutter_angleChanged,  turntableSendData, &TurntableSendDataHex::recvoutter_angle, Qt::QueuedConnection);
    QObject::connect(turntableData, &TurntableDataHex::myoutter_angleChanged,  turntablePort, &SerialPortTurntableHex::recvoutter_angle, Qt::QueuedConnection);

    // ── BD: 主线程 Data → 工作线程 Worker ──
    QObject::connect(bdData, &BDData::requestOpenPort,  bdPort, &SerialPortBD::onOpenPort,  Qt::QueuedConnection);
    QObject::connect(bdData, &BDData::requestClosePort, bdPort, &SerialPortBD::onClosePort, Qt::QueuedConnection);
    QObject::connect(bdData, &BDData::requestScanPorts, bdPort, &SerialPortBD::onScanPorts, Qt::QueuedConnection);

    // ── BD: 工作线程 Worker → 主线程 Data ──
    QObject::connect(bdPort, &SerialPortBD::portOpened,   bdData, &BDData::setPortOpen, Qt::QueuedConnection);
    QObject::connect(bdPort, &SerialPortBD::portClosed,   bdData, [bdData]{ bdData->setPortOpen(false); }, Qt::QueuedConnection);
    QObject::connect(bdPort, &SerialPortBD::portError,    bdData, &BDData::setError,    Qt::QueuedConnection);
    QObject::connect(bdPort, &SerialPortBD::portsChanged, bdData, &BDData::setPortList, Qt::QueuedConnection);
    QObject::connect(bdPort, &SerialPortBD::bdFrameReceived, bdData, &BDData::updateFromFrame, Qt::QueuedConnection);

    // ── CCD: 主线程 Data → 工作线程 Worker ──
    QObject::connect(ccdData, &CCDData::requestOpenPort,  ccdPort, &SerialPortCCD::onOpenPort,  Qt::QueuedConnection);
    QObject::connect(ccdData, &CCDData::requestClosePort, ccdPort, &SerialPortCCD::onClosePort, Qt::QueuedConnection);
    QObject::connect(ccdData, &CCDData::requestScanPorts, ccdPort, &SerialPortCCD::onScanPorts, Qt::QueuedConnection);
    // CCD 指令信号
    QObject::connect(ccdData, &CCDData::req30XFocus,        ccdPort, &SerialPortCCD::send30XFocus,        Qt::QueuedConnection);
    QObject::connect(ccdData, &CCDData::req1XFocus,         ccdPort, &SerialPortCCD::send1XFocus,         Qt::QueuedConnection);
    QObject::connect(ccdData, &CCDData::reqdigZoomopen,     ccdPort, &SerialPortCCD::senddigZoomopen,     Qt::QueuedConnection);
    QObject::connect(ccdData, &CCDData::reqdigZoomclose,    ccdPort, &SerialPortCCD::senddigZoomclose,    Qt::QueuedConnection);
    QObject::connect(ccdData, &CCDData::reqBacklightopen,   ccdPort, &SerialPortCCD::sendBacklightopen,   Qt::QueuedConnection);
    QObject::connect(ccdData, &CCDData::reqBacklightclose,  ccdPort, &SerialPortCCD::sendBacklightclose,  Qt::QueuedConnection);
    QObject::connect(ccdData, &CCDData::reqResolutionchange, ccdPort, &SerialPortCCD::sendResolutionchange, Qt::QueuedConnection);

    // ── CCD: 工作线程 Worker → 主线程 Data ──
    QObject::connect(ccdPort, &SerialPortCCD::portOpened,   ccdData, &CCDData::setPortOpen, Qt::QueuedConnection);
    QObject::connect(ccdPort, &SerialPortCCD::portClosed,   ccdData, [ccdData]{ ccdData->setPortOpen(false); }, Qt::QueuedConnection);
    QObject::connect(ccdPort, &SerialPortCCD::portError,    ccdData, &CCDData::setError,    Qt::QueuedConnection);
    QObject::connect(ccdPort, &SerialPortCCD::portsChanged, ccdData, &CCDData::setPortList, Qt::QueuedConnection);


    // ── TemplateBinding: 主线程 Data → 网络线程 Worker ──
    QObject::connect(templateBindingData, &TemplateBindingData::requestConnect,    networkWorker, &TemplateBindingWorker::onConnect,    Qt::QueuedConnection);
    QObject::connect(templateBindingData, &TemplateBindingData::requestDisconnect, networkWorker, &TemplateBindingWorker::onDisconnect, Qt::QueuedConnection);
    QObject::connect(templateBindingData, &TemplateBindingData::requestSendImages, networkWorker, &TemplateBindingWorker::onSendImages,  Qt::QueuedConnection);
    QObject::connect(templateBindingData, &TemplateBindingData::requestSendTxt,    networkWorker, &TemplateBindingWorker::onSendTxt,     Qt::QueuedConnection);

    //模式控制器的信号连接
    // 模式控制器 → 各串口线程（运行模式 / 外引导源 / 跟踪周期 变更通知）
    QObject::connect(&m_modeController, &ModeController::modeChanged, _myhandle, &Myhandle::modechanged, Qt::QueuedConnection);
    QObject::connect(&m_modeController, &ModeController::modeChanged, laserPort, &SerialPortLaser::ExmodeChanged, Qt::QueuedConnection);
    QObject::connect(&m_modeController, &ModeController::modeChanged, imagePort, &SerialPortImage::ExmodeChanged, Qt::QueuedConnection);
    QObject::connect(&m_modeController, &ModeController::modeChanged, turntablePort, &SerialPortTurntableHex::ProgramModeChanged, Qt::QueuedConnection);
    QObject::connect(&m_modeController, &ModeController::modeChanged, ccdPort, &SerialPortCCD::ExmodeChanged, Qt::QueuedConnection);

    QObject::connect(&m_modeController, &ModeController::exguideSrcChanged, laserPort, &SerialPortLaser::ExmodeChanged, Qt::QueuedConnection);
    QObject::connect(&m_modeController, &ModeController::exguideSrcChanged, imagePort, &SerialPortImage::ExmodeChanged, Qt::QueuedConnection);
    QObject::connect(&m_modeController, &ModeController::exguideSrcChanged, ccdPort, &SerialPortCCD::ExmodeChanged, Qt::QueuedConnection);

    QObject::connect(&m_modeController, &ModeController::exguideSettingChanged, laserPort, &SerialPortLaser::ExmodeChanged, Qt::QueuedConnection);
    QObject::connect(&m_modeController, &ModeController::exguideSettingChanged, imagePort, &SerialPortImage::ExmodeChanged, Qt::QueuedConnection);
    QObject::connect(&m_modeController, &ModeController::exguideSettingChanged, ccdPort, &SerialPortCCD::ExmodeChanged, Qt::QueuedConnection);

    // 外引导源串口未打开时，弹窗提示（工作线程 → 主线程 Data → QML 弹窗）
    QObject::connect(laserPort, &SerialPortLaser::exguideSerialNotOpen, laserData, &LaserData::popupMessage, Qt::QueuedConnection);
    QObject::connect(imagePort, &SerialPortImage::exguideSerialNotOpen, imageData, &ImageData::popupMessage, Qt::QueuedConnection);
    QObject::connect(ccdPort, &SerialPortCCD::exguideSerialNotOpen, ccdData, &CCDData::popupMessage, Qt::QueuedConnection);

    //手柄信号连接
    QObject::connect(_myhandle, &Myhandle::handleModeSignal, turntablePort, &SerialPortTurntableHex::sendHandleMode, Qt::QueuedConnection);

    //QML gamepad与手柄信号连接
    QObject::connect(m_gamepadBridge, &GamepadBridge::axisLeftXChange, _myhandle, &Myhandle::axisLeftXChanged, Qt::QueuedConnection);
    QObject::connect(m_gamepadBridge, &GamepadBridge::axisLeftYChange, _myhandle, &Myhandle::axisLeftYChanged, Qt::QueuedConnection);
    QObject::connect(m_gamepadBridge, &GamepadBridge::axisRightXChange, _myhandle, &Myhandle::axisRightXChanged, Qt::QueuedConnection);
    QObject::connect(m_gamepadBridge, &GamepadBridge::buttonL2Change, _myhandle, &Myhandle::buttonL2Changed, Qt::QueuedConnection);
    QObject::connect(m_gamepadBridge, &GamepadBridge::buttonR2Change, _myhandle, &Myhandle::buttonR2Changed, Qt::QueuedConnection);
    QObject::connect(m_gamepadBridge, &GamepadBridge::buttonAChange, _myhandle, &Myhandle::buttonAChanged, Qt::QueuedConnection);
    QObject::connect(m_gamepadBridge, &GamepadBridge::buttonBChange, _myhandle, &Myhandle::buttonBChanged, Qt::QueuedConnection);
    QObject::connect(m_gamepadBridge, &GamepadBridge::updateGamepad, _myhandle, &Myhandle::update, Qt::QueuedConnection);

    //为实现外引导模式进行的信号连接
    QObject::connect(imagePort, &SerialPortImage::reqTimesync, turntablePort, &SerialPortTurntableHex::sendTimesync, Qt::QueuedConnection);
    QObject::connect(laserPort, &SerialPortLaser::reqTimesync, turntablePort, &SerialPortTurntableHex::sendTimesync, Qt::QueuedConnection);
    QObject::connect(ccdPort, &SerialPortCCD::reqTimesync, turntablePort, &SerialPortTurntableHex::sendTimesync, Qt::QueuedConnection);
    QObject::connect(imagePort, &SerialPortImage::reqExsend_1s, turntablePort, &SerialPortTurntableHex::sendTrackMode_1s, Qt::QueuedConnection);
    QObject::connect(laserPort, &SerialPortLaser::reqExsend_1s, turntablePort, &SerialPortTurntableHex::sendTrackMode_1s, Qt::QueuedConnection);
    QObject::connect(ccdPort, &SerialPortCCD::reqExsend_1s, turntablePort, &SerialPortTurntableHex::sendTrackMode_1s, Qt::QueuedConnection);

    QObject::connect(imagePort, &SerialPortImage::reqExsend_5ms, turntablePort, &SerialPortTurntableHex::sendTrackMode_5ms, Qt::QueuedConnection);
    QObject::connect(laserPort, &SerialPortLaser::reqExsend_5ms, turntablePort, &SerialPortTurntableHex::sendTrackMode_5ms, Qt::QueuedConnection);
    QObject::connect(ccdPort, &SerialPortCCD::reqExsend_5ms, turntablePort, &SerialPortTurntableHex::sendTrackMode_5ms, Qt::QueuedConnection);

    // 目标中心坐标：StreamProcessor → CCD 串口线程（待 StreamProcessor 实例化后启用）
    // QObject::connect(streamProc, &StreamProcessor::targetCenterChanged,
    //                  ccdPort,    &SerialPortCCD::recvTargetCenter,
    //                  Qt::QueuedConnection);

    // ═══ 3) 创建线程并迁移 Worker ═══
    QThread *Laserthread = new QThread;
    QThread *Imagethread = new QThread;
    QThread *Turntablethread = new QThread;
    QThread *Handlethread = new QThread;
    QThread *BDthread = new QThread;
    QThread *CCDthread = new QThread;
    QThread *NetworkThread = new QThread;
    laserPort->moveToThread(Laserthread);
    imagePort->moveToThread(Imagethread);
    turntablePort->moveToThread(Turntablethread);
    bdPort->moveToThread(BDthread);
    ccdPort->moveToThread(CCDthread);
    _myhandle->moveToThread(Handlethread);
    networkWorker->moveToThread(NetworkThread);

    QObject::connect(Laserthread, &QThread::started, laserPort, &SerialPortLaser::dowork);
    QObject::connect(Imagethread, &QThread::started, imagePort, &SerialPortImage::dowork);
    QObject::connect(Turntablethread, &QThread::started, turntablePort, &SerialPortTurntableHex::dowork);
    QObject::connect(BDthread, &QThread::started, bdPort, &SerialPortBD::dowork);
    QObject::connect(CCDthread, &QThread::started, ccdPort, &SerialPortCCD::dowork);

    // 线程退出 → 先删 worker（已无事件循环在使用） → 再删线程自身
    QObject::connect(Laserthread, &QThread::finished, laserPort,    &QObject::deleteLater);
    QObject::connect(Laserthread, &QThread::finished, Laserthread,  &QObject::deleteLater);
    QObject::connect(Imagethread, &QThread::finished, imagePort,    &QObject::deleteLater);
    QObject::connect(Imagethread, &QThread::finished, Imagethread,  &QObject::deleteLater);
    QObject::connect(Turntablethread, &QThread::finished, turntablePort,    &QObject::deleteLater);
    QObject::connect(Turntablethread, &QThread::finished, Turntablethread,  &QObject::deleteLater);
    QObject::connect(Handlethread,    &QThread::finished, _myhandle,        &QObject::deleteLater);
    QObject::connect(Handlethread,    &QThread::finished, Handlethread,     &QObject::deleteLater);
    QObject::connect(BDthread,       &QThread::finished, bdPort,           &QObject::deleteLater);
    QObject::connect(BDthread,       &QThread::finished, BDthread,         &QObject::deleteLater);
    QObject::connect(CCDthread,      &QThread::finished, ccdPort,          &QObject::deleteLater);
    QObject::connect(CCDthread,      &QThread::finished, CCDthread,        &QObject::deleteLater);
    QObject::connect(NetworkThread,  &QThread::finished, networkWorker,    &QObject::deleteLater);
    QObject::connect(NetworkThread,  &QThread::finished, NetworkThread,    &QObject::deleteLater);


    Laserthread->start();
    Imagethread->start();
    Turntablethread->start();
    Handlethread->start();
    BDthread->start();
    CCDthread->start();
    NetworkThread->start();
    
    return app.exec();
}
