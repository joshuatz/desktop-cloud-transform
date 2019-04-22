#ifndef HELPERS_H
#define HELPERS_H

#include <QObject>
#include <QtNetwork>

class Helpers : public QObject
{
    Q_OBJECT
public:
    explicit Helpers(QObject *parent = nullptr);
    static void checkInternetConnection(bool *res);
    static QUrlQuery generateUrlQueryFromVarMap(QMap<QString,QVariant> params);
signals:
public slots:
    static bool checkValidImageFilePath(QString imagePath);
    static void copyToClipboard(QString strToCopy);
    static QString formatFilePathForQml(QString filePath);
    static QString forceEndingSlash(QString input);
    static QString removeBeginSlash(QString input);
    static bool getIsDebug();
private:
};

#endif // HELPERS_H
