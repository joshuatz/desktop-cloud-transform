#include "cloudinary.h"
#include <QDateTime>
#include <QDebug>
#include <QCryptographicHash>

Cloudinary::Cloudinary()
{

}

QString Cloudinary::generateSignature(QMap<QString,QVariant> paramsToSign, QString apiSecret){
    QList<QString> excludedParams = {"file","resource_type","api_key"};
    QString unhashedSignature = "";
    // Check for timestamp in params
    if (paramsToSign.contains("timestamp")==false){
        paramsToSign.insert("timestamp",QVariant(QDateTime::currentDateTime().toMSecsSinceEpoch()));
    }
    // Iterate through pass param map. Note - QMap should be automatically sorted by key already.
    QMap<QString,QVariant>::iterator i;
    int index = 0;
    for (i = paramsToSign.begin(); i != paramsToSign.end(); ++i){
        unhashedSignature += ((index > 0) ? "&" : "") + i.key() + "=" + i.value().toString();
        index++;
    }
    // Now append the api key directly (no & to join)
    unhashedSignature += apiSecret;
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
