#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "serialport/serialport_laser.h"
#include "serialport/serialport_image.h"
#include "serialport/serialport_turntable.h"
#include "vlcvideo/VlcVideoItem.h"
//使用VLC-QT
// #include <VLCQtCore/Common.h>
// #include <VLCQtQml/QmlVideoPlayer.h>
#include "video/vlcplayer.h"
// #include <VLCQtCore/Common.h>
// #include <VLCQtCore/Instance.h>
// #include <VLCQtCore/Media.h>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


    SerialPortLaser laserPort;
    SerialPortImage imagePort;
    SerialPortTurntable turntablePort;
    // VLCPlayer myvlcplayer;
    //"file:///E:/QTproject/vlclib/plugins"
    // VlcCommon::setPluginPath("file:///E:/QTproject/vlclib/plugins");
//     QStringList args = VlcCommon::args();
//     args << "--vout=dummy" << "--no-osd" ;
//     VlcInstance *instance = new VlcInstance(args, &app);
//     if (!instance->status()) {
//     qCritical() << "VLC instance creation failed!";
//     return -1;
//    }

    QQmlApplicationEngine engine;

    engine.addImportPath(TaoQuickImportPath);
    engine.addImportPath("E:/QTproject/vlc-qt/install/qml");
    engine.addImportPath(app.applicationDirPath());
    engine.rootContext()->setContextProperty("taoQuickImportPath", TaoQuickImportPath);
    engine.rootContext()->setContextProperty("laserSerial", &laserPort);
    engine.rootContext()->setContextProperty("imageSerial", &imagePort);
    engine.rootContext()->setContextProperty("turntableSerial", &turntablePort);
    qmlRegisterType<VlcVideoItem>("VlcVideo", 1, 0, "VlcVideo");
    // engine.rootContext()->setContextProperty("vlcplayer", &myvlcplayer);//
    // engine.addImportPath(app.applicationDirPath() + "/qml");
    // engine.rootContext()->setContextProperty("vlcInstance", instance);
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
