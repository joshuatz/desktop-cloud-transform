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
    struct CloudinaryUsageBreakdownWithPercent {
        int usage;
        int limit;
        double used_percentage;
    };
    struct CloudinaryUsageBreakdown {
        int usage;
        int limit;
    };

    struct cloudinaryUsage {
        QString plan;
        QString last_updated;
        Cloudinary::CloudinaryUsageBreakdown transformations;
        Cloudinary::CloudinaryUsageBreakdownWithPercent objects;
        Cloudinary::CloudinaryUsageBreakdownWithPercent bandwidth;
        Cloudinary::CloudinaryUsageBreakdownWithPercent storage;
        int requests;
        int resources;
        int derived_resources;
        Cloudinary::CloudinaryUsageBreakdown url2png;
        Cloudinary::CloudinaryUsageBreakdown webpurify;
        Cloudinary::CloudinaryUsageBreakdown aspose;
        Cloudinary::CloudinaryUsageBreakdown imagga_tagging;
        Cloudinary::CloudinaryUsageBreakdown adv_face;
    };
    static QString generateSignature(QMap<QString,QVariant> paramsToSign,QString apiSecret);
    static void uploadRemoteFileByUrl(QString publicImageUrl);
    static void uploadLocalFileByPath(QString localImagePath);
    static void uploadFileByString(QString fileString);
    static void uploadFileByParams(QMap<QString,QVariant> params);
    static void uploadFileByParamsWUploaderInstance(QMap<QString,QVariant> params,Uploader *uploaderInstance);
    static void getUsageInfoJson(void(*fnPtr)(QJsonObject res));
    static QString generateImageUrlFromConfigAndId(QString uploadedPublicId, TransformationConfig config);
    typedef QMap<QString,QVariant> cloudinaryParams;
    static QString getPublicUploadUrlBase();
    static void deleteFileById(QString id,Uploader *uploaderInstance);
private:
    static QString getUploadEndpoint(QString resourceType);
    static QString getAdminBaseEndpoint();
    static QString getBasicAuthHeaderString();
    static QString getFetchEndpoint();
    static QString getDeleteByPublicIdsEndpoint(QList<QString> publicIds);
};

#endif // CLOUDINARY_H
