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
    if (Helpers::getIsDebug()){
        qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
    }
    QCoreApplication::addLibraryPath("./");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Register settings config
    QCoreApplication::setOrganizationName("JoshuaTzucker");
    QCoreApplication::setOrganizationDomain("joshuatz.com");
    QCoreApplication::setApplicationName("Desktop Cloud Transform");
    QCoreApplication::setApplicationVersion("0.0.1");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Establish SQLite - this should create the .db file if it does not exist
    Database db = Database();

    // Load global settings into memory
    GlobalSettings::getInstance()->loadFromStorage();
    // Load configs
    TransformationList::getInstance()->loadAllFromStorage();
    // Load stats
    Stats::getInstance()->forceRefreshFromDb();

    // Set Material theme for QTQuickControls
    QQuickStyle::setStyle("Material");

    // Create class instances
    Helpers myHelpers;

    // Register contexts
    engine.rootContext()->setContextProperty("GlobalSettings",GlobalSettings::getInstance());
    engine.rootContext()->setContextProperty("Helpers",&myHelpers);
    engine.rootContext()->setContextProperty("Uploader",Uploader::getInstance());
    engine.rootContext()->setContextProperty("UploadConfigsList",TransformationList::getInstance());
    engine.rootContext()->setContextProperty("Stats",Stats::getInstance());
    // Register metatypes
    qRegisterMetaType<UploadActionResult>();
    qRegisterMetaType<TransformationConfig>();

    // Log startup :)
    Stats::getInstance()->logStat("application","startup",false);
    Stats::getInstance()->fireGaEvent("application","startup","",NULL);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
