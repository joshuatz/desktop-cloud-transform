#include "appinfo.h"
#include "helpers.h"
#include <QObject>
#include <QDebug>
#include <QImageReader>
#include <QtNetwork>
#include <QGuiApplication>
#include <QByteArray>
#include <QClipboard>

Helpers::Helpers(QObject *parent) : QObject(parent)
{

}

void Helpers::checkInternetConnection(bool *res){

    qDebug() << "Checking internet connection...";
    QNetworkAccessManager *netManager = new QNetworkAccessManager();

    // Use lambda as receiver with connect to get result of network finished event
    QObject::connect(netManager,&QNetworkAccessManager::finished,[=](QNetworkReply *reply) {
        if (reply->error()){
            *res = false;
        }
        else {
            *res = true;
        }
        reply->deleteLater();
    });

    // Start the network request
    QNetworkReply *netReply = netManager->get(QNetworkRequest(QString("https://www.google.com/")));
}

bool Helpers::checkValidImageFilePath(QString imagePath){
    // @TODO combine with additonal check of MIME type and generic can-access-file check.
    // Currently failing if image extension != detected mime type
    QImageReader reader(imagePath);
    if (reader.canRead()){
        return true;
    }
    // Note: reader.canRead will return false even if valid image, if detected Mime (via magic bytes) does not match extension (e.g. .png)
    QFile file(imagePath);
    // test if file is readable - you need to actually open file to check this
    file.open(QIODevice::ReadOnly);
    qDebug() << "file.isReadable()" << file.isReadable();
    if (file.isReadable()){
        // Make sure mime type is valid image
        QList<QByteArray> supportedImageMimeTypes = QImageReader::supportedMimeTypes();
        QMimeDatabase mimeDb;
        QMimeType fileContentMimeType = mimeDb.mimeTypeForFile(imagePath,QMimeDatabase::MatchContent);
        if (supportedImageMimeTypes.contains(QByteArray(fileContentMimeType.name().toLocal8Bit()))){
            file.close();
            return true;
        }
        // Also check the aliases of the primary mime type
        QStringList fileContentMimeTypeAliases = fileContentMimeType.aliases();
        for (int i=0; i<fileContentMimeTypeAliases.length(); ++i){
            if (supportedImageMimeTypes.contains(QByteArray(fileContentMimeTypeAliases.at(i).toLocal8Bit()))){
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

QUrlQuery Helpers::generateUrlQueryFromVarMap(QMap<QString, QVariant> params){
    QUrlQuery query;
    QMap<QString,QVariant>::iterator i;
    for (i = params.begin(); i != params.end(); ++i){
        query.addQueryItem(i.key(),i.value().toString());
    }
    return query;
}

void Helpers::copyToClipboard(QString strToCopy){
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(strToCopy);
}

QString Helpers::formatFilePathForQml(QString filePath){
    // Replace "\" with "/"
    QString formattedPath = filePath.replace("\\","/");
    // Make sure it is prefixed with "file:///"
    if (formattedPath.startsWith("file:")==false){
        formattedPath = "file:///" + formattedPath;
    }
    return formattedPath;
}

QString Helpers::forceEndingSlash(QString input){
    QString output = input;
    if (output.endsWith("/")==false){
        output+= "/";
    }
    return output;
}

QString Helpers::removeBeginSlash(QString input){
    QString output = input;
    if (output.startsWith("/")){
        output.remove(QRegularExpression("^\\/"));
    }
    return output;
}

bool Helpers::getIsDebug(){
#ifdef QT_DEBUG
    return true;
#else
    return false;
#endif
}

QString Helpers::getVersionString(){
    return APP_VERSION_STRING;
}
