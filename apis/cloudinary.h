#ifndef CLOUDINARY_H
#define CLOUDINARY_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QtNetwork>

class Cloudinary
{
public:
    Cloudinary();
    static QString generateSignature(QMap<QString,QVariant> paramsToSign,QString apiSecret);
    static void uploadRemoteFileByUrl(QString publicImageUrl);
    static void uploadLocalFileByPath(QString localImagePath);
    static void uploadFileByString(QString fileString);
    static void uploadFileByParams(QMap<QString,QVariant> params);
private:
    static QString getUploadEndpoint(QString resourceType);
};

#endif // CLOUDINARY_H
