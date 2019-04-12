#include "uploader.h"
#include "helpers.h"
#include "apis/cloudinary.h"
#include <QClipboard>
#include <QGuiApplication>

Uploader::Uploader(QObject *parent) : QObject(parent)
{

}

Uploader *Uploader::m_instance = nullptr;

Uploader *Uploader::getInstance(){
    if (m_instance == nullptr){
        m_instance = new Uploader();
    }
    return m_instance;
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
    qDebug() << "Running in slot of Uploader class!";
    QString data = (QString) reply->readAll();
    qDebug() << data;
    UploadActionResult result;
    if (reply->error()){
        result.success = false;
    }
    else {
        result.success = true;
        QJsonObject jsonResult = QJsonObject(QJsonDocument::fromJson(data.toUtf8()).object());
        // Parse the reply as JSON and map to result
        result.url = jsonResult["url"].toString();
        result.id = jsonResult["public_id"].toString();
        // Copy url to clipboard
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(result.url);
    }
    // Push the result
    this->m_lastUploadActionResult = result;
    emit this->uploadActionResultReceived();
    // Finally, regardless of actual reply, close out the pending upload
    this->setUploadInProgress(false);
}

void Uploader::setUploadInProgress(bool uploadInProgressStatus){
    if (m_uploadInProgress != uploadInProgressStatus){
        m_uploadInProgress = uploadInProgressStatus;
        emit Uploader::uploadInProgressChanged();
    }
}


UploadActionResult Uploader::getLastUploadResult(){
    return m_lastUploadActionResult;
}
