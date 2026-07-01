#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include "serialport/serialport_laser.h"
#include "serialport/serialport_image.h"
#include "serialport/serialport_turntable.h"
#include "vlcvideo/VlcVideoItem.h"
#include "handle/myhandle.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // 注册自定义结构体到 Qt 元对象系统（QueuedConnection 跨线程传递必需）
    qRegisterMetaType<laser_send_frame>("laser_send_frame");
    qRegisterMetaType<laser_recv_frame>("laser_recv_frame");
    qRegisterMetaType<image_send_frame>("image_send_frame");
    qRegisterMetaType<image_recv_frame>("image_recv_frame");

    // ═══ 主线程对象：QML 直接访问 ═══
    LaserData *laserData = new LaserData(&app);
    LaserSendData *laserSendData = new LaserSendData(&app);
    ImageData *imageData = new ImageData(&app);
    ImageSendData *imageSendData = new ImageSendData(&app);
    TurntableData * turntableData = new TurntableData(&app);
    TurntableSendData *turntableSendData = new TurntableSendData(&app);

    // ═══ 工作线程对象：只处理串口 I/O ═══
    SerialPortLaser *laserPort = new SerialPortLaser;       // 无父对象
    SerialPortImage *imagePort = new SerialPortImage;
    SerialPortTurntable *turntablePort = new SerialPortTurntable;

    // 把 Data 对象挂给 Worker 存引用（parseData 需要 m_laserData->updateFromFrame）
    laserPort->m_laserData = laserData;
    laserPort->m_laserSendData = laserSendData;
    imagePort->m_imageData = imageData;
    imagePort->m_imageSendData = imageSendData;
    turntablePort->m_turntableData = turntableData;
    turntablePort->m_turntableSendData = turntableSendData;
    //创建手柄对象
    Myhandle _myhandle(&app);

    // ═══ 1) 先加载 QML，建立绑定 ═══
    QQmlApplicationEngine engine;
    engine.addImportPath(TaoQuickImportPath);
    engine.addImportPath(app.applicationDirPath());
    engine.rootContext()->setContextProperty("taoQuickImportPath", TaoQuickImportPath);
    engine.rootContext()->setContextProperty("laserData", laserData);
    engine.rootContext()->setContextProperty("laserSendData", laserSendData);
    engine.rootContext()->setContextProperty("imageData", imageData);
    engine.rootContext()->setContextProperty("imageSendData", imageSendData);
    engine.rootContext()->setContextProperty("handle", &_myhandle);
    engine.rootContext()->setContextProperty("turntableData", turntableData);
    engine.rootContext()->setContextProperty("turntableSendData", turntableSendData);
    qmlRegisterType<VlcVideoItem>("VlcVideo", 1, 0, "VlcVideo");

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

    // ═══ 3) 创建线程并迁移 Worker ═══
    QThread *Laserthread = new QThread;
    QThread *Imagethread = new QThread;
    QThread *Turntablethread = new QThread;
    laserPort->moveToThread(Laserthread);
    imagePort->moveToThread(Imagethread);
    turntablePort->moveToThread(Turntablethread);

    QObject::connect(Laserthread, &QThread::started, laserPort, &SerialPortLaser::dowork);
    QObject::connect(Imagethread, &QThread::started, imagePort, &SerialPortImage::dowork);
    QObject::connect(Turntablethread, &QThread::started, turntablePort, &SerialPortTurntable::dowork);

    // 线程退出 → 先删 worker（已无事件循环在使用） → 再删线程自身
    QObject::connect(Laserthread, &QThread::finished, laserPort,    &QObject::deleteLater);
    QObject::connect(Laserthread, &QThread::finished, Laserthread,  &QObject::deleteLater);
    QObject::connect(Imagethread, &QThread::finished, imagePort,    &QObject::deleteLater);
    QObject::connect(Imagethread, &QThread::finished, Imagethread,  &QObject::deleteLater);
    QObject::connect(Turntablethread, &QThread::finished, turntablePort,    &QObject::deleteLater);


    Laserthread->start();
    Imagethread->start();
    Turntablethread->start();
    
    return app.exec();
}
