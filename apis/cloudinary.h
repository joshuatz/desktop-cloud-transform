#ifndef CLOUDINARY_H
#define CLOUDINARY_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QtNetwork>
#include "models/uploader.h"

class Cloudinary
{
public:
    Cloudinary();
    static QString generateSignature(QMap<QString,QVariant> paramsToSign,QString apiSecret);
    static void uploadRemoteFileByUrl(QString publicImageUrl);
    static void uploadLocalFileByPath(QString localImagePath);
    static void uploadFileByString(QString fileString);
    static void uploadFileByParams(QMap<QString,QVariant> params);
    static void uploadFileByParamsWUploaderInstance(QMap<QString,QVariant> params,Uploader *uploaderInstance);
    static QString generateImageUrlFromConfigAndId(QString uploadedPublicId, TransformationConfig config);
    typedef QMap<QString,QVariant> cloudinaryParams;
private:
    static QString getUploadEndpoint(QString resourceType);
};

#endif // CLOUDINARY_H
