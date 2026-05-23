#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "serialport/serialport_laser.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<SerialPortLaser>("SerialPort", 1, 0, "SerialPortLaser");

    SerialPortLaser laserPort;

    QQmlApplicationEngine engine;

    engine.addImportPath(TaoQuickImportPath);
    engine.rootContext()->setContextProperty("taoQuickImportPath", TaoQuickImportPath);
    engine.rootContext()->setContextProperty("laserSerial", &laserPort);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
