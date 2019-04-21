#include "cloudinary.h"
#include "globalsettings.h"
#include <QDateTime>
#include <QDebug>
#include <QCryptographicHash>
#include <QtNetwork>
#include <QFile>
#include <QfileInfo>
#include "helpers.h"
#include "models/uploader.h"

Cloudinary::Cloudinary()
{

}

QString Cloudinary::generateSignature(QMap<QString,QVariant> paramsToSign, QString apiSecret){
    QList<QString> excludedParams = {"file","filepath","resource_type","api_key"};
    QString unhashedSignature = "";
    // Check for timestamp in params
    if (paramsToSign.contains("timestamp")==false){
        paramsToSign.insert("timestamp",QVariant(QDateTime::currentDateTime().toMSecsSinceEpoch()));
    }
    // Iterate through pass param map. Note - QMap should be automatically sorted by key already.
    QMap<QString,QVariant>::iterator i;
    int index = 0;
    for (i = paramsToSign.begin(); i != paramsToSign.end(); ++i){
        if (excludedParams.contains(i.key())==false){
            unhashedSignature += ((index > 0) ? "&" : "") + i.key() + "=" + i.value().toString();
            index++;
        }
    }
    // Now append the api key directly (no & to join)
    unhashedSignature += apiSecret;
    qDebug() << unhashedSignature;
    // return sha-1'ed output of signature (digest)
    return QCryptographicHash::hash(unhashedSignature.toUtf8(),QCryptographicHash::Sha1).toHex();
}

/**
 * For generating signatures:
 *      - https://github.com/cloudinary/cloudinary_npm/blob/2bc4df37283ca0dcd4516c0ceac31c06d20be1ba/lib-es5/utils/index.js#L998
 *      - https://github.com/cloudinary/cloudinary_php/blob/81c075b7f65fc255cadbf590bee962a457261380/src/Cloudinary.php#L1169
 *      - https://github.com/cloudinary/CloudinaryDotNet/blob/99afd4c7b89544484b4a5bf2bb00e2c0307156a1/Shared/ApiShared.cs#L374
 *      - https://github.com/cloudinary/cloudinary_java/blob/e1b363218f89df9ebe92a86ed64ce7ffd2c25a09/cloudinary-core/src/main/java/com/cloudinary/Cloudinary.java#L129
 *
 */

void Cloudinary::uploadLocalFileByPath(QString localImagePath){
    QMap<QString,QVariant> params;
    params.insert("filepath",localImagePath);
    uploadFileByParams(params);
}

void Cloudinary::uploadFileByParams(QMap<QString, QVariant> params){
    Cloudinary::uploadFileByParamsWUploaderInstance(params,Uploader::getInstance());
}

void Cloudinary::uploadFileByParamsWUploaderInstance(QMap<QString, QVariant> params, Uploader *uploaderInstance){
    QMimeDatabase mimeDb;
    bool mock = false;

    // check for timestamp
    if (params.contains("timestamp")==false){
        long long timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        params.insert("timestamp",timestamp);
    }
    // Check for auth
    if (params.contains("signature")==false){
        params.insert("signature",generateSignature(params,GlobalSettings::getInstance()->getCloudinaryApiSecret()));
    }
    if (params.contains("api_key")==false){
        params.insert("api_key",GlobalSettings::getInstance()->getCloudinaryApiKey());
    }

    if (mock){
        params.insert("api_key","foobar");
    }
    // Construct a multi-part form request, starting with the actual form-data
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QMap<QString,QVariant>::iterator i;
    for (i = params.begin(); i != params.end(); ++i){
        if ((i.key()=="filepath" && params.contains("file")==false) || (i.key()=="file" && i.value().type() == QVariant::String && Helpers::checkValidImageFilePath(i.value().toString()))){
            // Let other funcs pass file as a filepath string instead of raw file
            QString filePath = i.value().toString();
            QHttpPart imagePart;
            QFile *file = new QFile(filePath);
            QString filename = QFileInfo(filePath).fileName();
            // Note - image form part needs "filename" attribute on content-disposition, and then the image mime type on Content-Type header
            imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + filename + "\""));
            imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeDb.mimeTypeForFile(filePath,QMimeDatabase::MatchContent).name()));
            file->open(QIODevice::ReadOnly);
            imagePart.setBodyDevice(file);
            file->setParent(multiPart);
            multiPart->append(imagePart);
        }
        else if (i.key()=="file" && (i.value().type() == QVariant::ByteArray || i.value().type() == QMetaType::QByteArray)){
            QHttpPart imagePart;
            imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\""));
            imagePart.setBody(i.value().toByteArray());
            multiPart->append(imagePart);
        }
        else if (i.value().canConvert(QMetaType::QString) || i.value().canConvert(QMetaType::Int)) {
            // Normal form section
            QHttpPart textPart;
            textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + i.key() + "\""));
            const QString textVal = i.value().toString();
            textPart.setBody(i.value().toByteArray());
            multiPart->append(textPart);
        }
    }
    qDebug() << "About to make POST to Cloudinary with form-data";
    Uploader::getInstance()->setUploadInProgress(true);
    QNetworkAccessManager *netManager = new QNetworkAccessManager();
    QNetworkRequest request;
    if (mock){
        request = QNetworkRequest(QString("http://requestbin.fullcontact.com/15cpiif1"));
    }
    else {
        request = QNetworkRequest(Cloudinary::getUploadEndpoint("image"));
    }


    // Set up connect to listen for result
//    QObject::connect(netManager,&QNetworkAccessManager::finished,Uploader::getInstance(),&Uploader::receiveNetworkReply);
    QObject::connect(netManager,&QNetworkAccessManager::finished,uploaderInstance,&Uploader::receiveNetworkReply);
//    QObject::connect(netManager,&QNetworkAccessManager::finished,[=](QNetworkReply *finishedReply) {
//        uploaderInstance->receiveNetworkReply()
//    });

    QNetworkReply *reply = netManager->post(request, multiPart);

    qDebug() << reply;
    //multiPart->setParent(reply); // delete the multiPart with the reply
}

void Cloudinary::uploadRemoteFileByUrl(QString publicImageUrl){
    QNetworkAccessManager *netManager = new QNetworkAccessManager();
    long long timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // Generate signature
    QMap<QString,QVariant> params;
    params.insert("timestamp",timestamp);
    params.insert("file",QVariant(publicImageUrl));
    params.insert("api_key",GlobalSettings::getInstance()->getCloudinaryApiKey());
    QString signature = generateSignature(params,GlobalSettings::getInstance()->getCloudinaryApiSecret());
    // Add signature to the params
    params.insert("signature",signature);
    // Construct POST
    QUrlQuery postData = Helpers::generateUrlQueryFromVarMap(params);
    QNetworkRequest request(Cloudinary::getUploadEndpoint("image"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    netManager->post(request,postData.toString(QUrl::FullyEncoded).toUtf8());
}

QString Cloudinary::getAdminBaseEndpoint(){
    return "https://api.cloudinary.com/v1_1/" + GlobalSettings::getInstance()->getCloudinaryCloudName();
}

QString Cloudinary::getUploadEndpoint(QString resourceType){
    return "https://api.cloudinary.com/v1_1/" + GlobalSettings::getInstance()->getCloudinaryCloudName()  + "/" + resourceType + "/upload";
}

QString Cloudinary::getPublicUploadUrlBase(){
    return "https://res.cloudinary.com/" + GlobalSettings::getInstance()->getCloudinaryCloudName() + "/image/upload";
}

QString Cloudinary::getFetchEndpoint(){
    return "https://res.cloudinary.com/" + GlobalSettings::getInstance()->getCloudinaryCloudName() + "/image/fetch";
}

QString Cloudinary::generateImageUrlFromConfigAndId(QString uploadedPublicId, TransformationConfig config){
    // Construct the base URL
    QString publicUrl = Helpers::forceEndingSlash(Cloudinary::getPublicUploadUrlBase());
    if (config.usesPreset){
        // This can only be used with uploads, not with public URLs
    }
    else if (config.usesNamedTransformation){
        publicUrl += "t_" + config.namedTransformation + "/";
    }
    else if (config.usesTransformationRawString){
        publicUrl += config.transformationRawString;
    }
    // make sure that URL ends in a slash before appending the base image
    if (publicUrl.endsWith("/")==false){
        publicUrl += "/";
    }
    // Finally, append the public ID or filename of the "base image" (previously uploaded image upon which to apply transformations)
    publicUrl += uploadedPublicId;

    return publicUrl;
}

QString Cloudinary::getBasicAuthHeaderString(){
    // Construct base64 section
    QString base64AuthString = (GlobalSettings::getInstance()->getCloudinaryApiKey() + ":" + GlobalSettings::getInstance()->getCloudinaryApiSecret()).toLocal8Bit().toBase64();
    return "Basic " + base64AuthString;
}

void Cloudinary::getUsageInfoJson(void(*fnPtr)(QJsonObject res)){
    if (fnPtr!=nullptr){
        QString endpoint = Cloudinary::getAdminBaseEndpoint() + "/usage";
        QNetworkAccessManager *netManager = new QNetworkAccessManager();
        QNetworkRequest request(endpoint);
        request.setRawHeader("Authorization",Cloudinary::getBasicAuthHeaderString().toLocal8Bit());
        QObject::connect(netManager,&QNetworkAccessManager::finished,[fnPtr](QNetworkReply *reply){
            QString data = reply->readAll();
            QJsonObject jsonResult = QJsonObject(QJsonDocument::fromJson(data.toUtf8()).object());
            fnPtr(jsonResult);
            reply->deleteLater();
        });
        QNetworkReply *reply = netManager->get(request);
    }
}

void Cloudinary::deleteFileById(QString id, Uploader *uploaderInstance){
    QString endpoint = Cloudinary::getDeleteByPublicIdsEndpoint(QList<QString>({id}));
    QNetworkAccessManager *netManager = new QNetworkAccessManager();
    QNetworkRequest request(endpoint);
    qDebug() << endpoint;
    request.setRawHeader("Authorization",Cloudinary::getBasicAuthHeaderString().toLocal8Bit());
    QObject::connect(netManager,&QNetworkAccessManager::finished,[uploaderInstance](QNetworkReply *reply){
        QString data = reply->readAll();
        QJsonObject jsonResult = QJsonObject(QJsonDocument::fromJson(data.toUtf8()).object());
        qDebug() << jsonResult;
        uploaderInstance->setDeletionInProgress(false);
        reply->deleteLater();
    });
    QNetworkReply *reply = netManager->deleteResource(request);
}

QString Cloudinary::getDeleteByPublicIdsEndpoint(QList<QString> publicIds){
    QString deleteUrl = "";
    if (publicIds.length() > 0){
        QString base = Helpers::forceEndingSlash(Cloudinary::getAdminBaseEndpoint());
        deleteUrl = base + "resources/image/upload?";
        for (int i = 0; i < publicIds.length(); ++i){
            deleteUrl += (i > 0 ? "&" : "");
            deleteUrl += "public_ids[]=" + publicIds.at(i);
        }
    }
    return deleteUrl;
}
