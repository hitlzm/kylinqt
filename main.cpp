#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "serialport/serialport_laser.h"
#include "serialport/serialport_image.h"
#include "serialport/serialport_turntable.h"
//使用VLC-QT
#include <VLCQtCore/Common.h>
#include <VLCQtQml/QmlVideoPlayer.h>
#include "video/vlcplayer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // qmlRegisterType<SerialPortLaser>("SerialPort", 1, 0, "SerialPortLaser");

    SerialPortLaser laserPort;
    SerialPortImage imagePort;
    SerialPortTurntable turntablePort;
    VLCPlayer myvlcplayer;

    // VlcCommon::setPluginPath(app.applicationDirPath() + "/plugins");
    // VlcQmlVideoPlayer::registerPlugin();
    // qputenv(
    // "E:/QTproject/vlclib/plugins",
    // "E:/QTproject/vlc-qt/install/bin/plugins"
    // );

    QQmlApplicationEngine engine;

    engine.addImportPath(TaoQuickImportPath);
    engine.addImportPath("E:/QTproject/vlc-qt/install/qml");
    engine.addImportPath(app.applicationDirPath());
    engine.rootContext()->setContextProperty("taoQuickImportPath", TaoQuickImportPath);
    engine.rootContext()->setContextProperty("laserSerial", &laserPort);
    engine.rootContext()->setContextProperty("imageSerial", &imagePort);
    engine.rootContext()->setContextProperty("turntableSerial", &turntablePort);
    engine.rootContext()->setContextProperty("vlcplayer", &myvlcplayer);
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
