#include "uploader.h"
#include "helpers.h"
#include "apis/cloudinary.h"

Uploader::Uploader(QObject *parent) : QObject(parent)
{

}

int Uploader::uploadImageFromLocalPath(QString localImageFilePath){
    // @TODO implement queue system
    int queueNumber = 0;
    if (Helpers::checkValidImageFilePath(localImageFilePath)){
        this->m_processingIndex = queueNumber;
        qDebug() << "Uploading image. in queue at #" << queueNumber;
        Cloudinary::uploadLocalFileByPath(localImageFilePath);
    }
    return queueNumber;
}

void Uploader::receiveNetworkReply(QNetworkReply *reply){
    QString data = (QString) reply->readAll();
    qDebug() << data;
}
