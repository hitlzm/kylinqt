#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include "serialport/serialport_laser.h"
#include "serialport/serialport_image.h"
#include "serialport/serialport_turntable.h"
#include "vlcvideo/VlcVideoItem.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    SerialPortLaser laserPort;
    SerialPortImage imagePort;
    // SerialPortTurntable turntablePort;

    // 1) 先加载 QML，在 moveToThread 之前建立所有信号连接
    QQmlApplicationEngine engine;

    engine.addImportPath(TaoQuickImportPath);
    engine.addImportPath(app.applicationDirPath());
    engine.rootContext()->setContextProperty("taoQuickImportPath", TaoQuickImportPath);
    engine.rootContext()->setContextProperty("laserSerial", &laserPort);
    engine.rootContext()->setContextProperty("imageSerial", &imagePort);
    // engine.rootContext()->setContextProperty("turntableSerial", &turntablePort);
    qmlRegisterType<VlcVideoItem>("VlcVideo", 1, 0, "VlcVideo");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            qCritical() << "Failed to load QML file";
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // 2) QML 连接建立完毕，创建线程并迁移
    // QThread *Laserthread = new QThread;
    // QThread *Imagethread = new QThread;
    // // QThread *Turntablethread = new QThread;
    // laserPort.moveToThread(Laserthread);
    // imagePort.moveToThread(Imagethread);
    // // turntablePort.moveToThread(Turntablethread);

    // QObject::connect(Laserthread, &QThread::started, &laserPort, &SerialPortLaser::dowork);
    // QObject::connect(Imagethread, &QThread::started, &imagePort, &SerialPortImage::dowork);
    // // QObject::connect(Turntablethread, &QThread::started, &turntablePort, &SerialPortTurntable::dowork);

    // // 3) 启动工作线程
    // Laserthread->start();
    // Imagethread->start();
    // Turntablethread->start();
    
    return app.exec();
}
