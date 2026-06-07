#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "serialport/serialport_laser.h"
#include "serialport/serialport_image.h"
#include "serialport/serialport_turntable.h"
#include "vlcvideo/VlcVideoItem.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // 串口对象留在主线程——QSerialPort 本身是异步事件驱动，不需要 worker 线程
    // Windows 上 QSerialPort 依赖 I/O 完成端口，内部有独立线程，跨线程父子对象会崩溃
    SerialPortLaser laserPort;
    SerialPortImage imagePort;
    SerialPortTurntable turntablePort;

    QQmlApplicationEngine engine;

    engine.addImportPath(TaoQuickImportPath);
    engine.addImportPath("E:/QTproject/vlc-qt/install/qml");
    engine.addImportPath(app.applicationDirPath());
    engine.rootContext()->setContextProperty("taoQuickImportPath", TaoQuickImportPath);
    engine.rootContext()->setContextProperty("laserSerial", &laserPort);
    engine.rootContext()->setContextProperty("imageSerial", &imagePort);
    engine.rootContext()->setContextProperty("turntableSerial", &turntablePort);
    qmlRegisterType<VlcVideoItem>("VlcVideo", 1, 0, "VlcVideo");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
