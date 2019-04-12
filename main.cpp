#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include "helpers.h"
#include "database.h"
#include "globalsettings.h"
#include "apis/cloudinary.h"

int main(int argc, char *argv[])
{
    // Establish SQLite - this should create the .db file if it does not exist
    Database db = Database();

    // Load global settings into memory
    GlobalSettings::getInstance()->loadFromStorage();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Set Material theme for QTQuickControls
    QQuickStyle::setStyle("Material");

//    QMap<QString,QVariant> paramMap;
//    paramMap.insert("TestKeyProp",QVariant("Test key val"));
//    qDebug() << Cloudinary::generateSignature(paramMap,"fakeapikey");

    // Register contexts
    engine.rootContext()->setContextProperty("GlobalSettings",GlobalSettings::getInstance());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
