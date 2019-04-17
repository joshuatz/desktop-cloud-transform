#include "apis/cloudinary.h"
#include "database.h"
#include "globalsettings.h"
#include "helpers.h"
#include "models/stats.h"
#include "models/transformationlist.h"
#include "models/transformationconfig.h"
#include "models/uploader.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    // Establish SQLite - this should create the .db file if it does not exist
    Database db = Database();

    // Load global settings into memory
    GlobalSettings::getInstance()->loadFromStorage();
    // Load configs
    TransformationList::getInstance()->loadAllFromStorage();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Set Material theme for QTQuickControls
    QQuickStyle::setStyle("Material");

    // Create class instances
    Helpers myHelpers;

    // Register contexts
    engine.rootContext()->setContextProperty("GlobalSettings",GlobalSettings::getInstance());
    engine.rootContext()->setContextProperty("Helpers",&myHelpers);
    engine.rootContext()->setContextProperty("Uploader",Uploader::getInstance());
    engine.rootContext()->setContextProperty("UploadConfigsList",TransformationList::getInstance());
    // Register metatypes
    qRegisterMetaType<UploadActionResult>();
    qRegisterMetaType<TransformationConfig>();

    // Register settings config
    QCoreApplication::setOrganizationName("JoshuaTzucker");
    QCoreApplication::setOrganizationDomain("joshuatz.com");
    QCoreApplication::setApplicationName("Desktop Cloud Transform");

    // Log startup :)
    Stats::getInstance()->logStat("application","startup",false);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
