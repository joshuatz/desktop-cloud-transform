#include "uploader.h"
#include "helpers.h"
#include "apis/cloudinary.h"
#include <QClipboard>
#include <QGuiApplication>
#include "transformationlist.h"
#include "downloader.h"
#include <QFile>
#include <QFileInfo>

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

void Uploader::uploadImageWithConfigId(QString localImageFilePath,int configId){
    TransformationConfig config = TransformationList::getConfigById(configId);
    this->uploadImageWithConfig(localImageFilePath,config);
}

void Uploader::uploadImageWithConfig(QString localImageFilePath,TransformationConfig config){
    QMap<QString,QVariant> params;
    params.insert("filepath",localImageFilePath);
    // Construct a new Uploader instance and attach config and flag it
    Uploader *uploaderInstance = new Uploader();
    uploaderInstance->m_hasAttachedConfig = true;
    uploaderInstance->m_attachedConfig = config;
    uploaderInstance->m_localFilePath = localImageFilePath;
    if (config.storeOriginal){
        // This means that two api calls will have to be made - one to upload the original file, and a second to apply a transformation on top of the original
        // Params should only contain filepath for first round
        // Set flag that says this is just round 1
        uploaderInstance->m_attachedConfigIsInProgress = true;
        // Call cloudinary uploader with instance and empty params
        Cloudinary::uploadFileByParamsWUploaderInstance(params,uploaderInstance);
    }
    else {
        // This is the final upload action.
        if (config.usesPreset){
            params.insert("upload_preset",QVariant(config.presetName));
        }
        else if (config.usesTransformationRawString){
            params.insert("transformation",QVariant(config.transformationRawString));
        }
        Cloudinary::uploadFileByParamsWUploaderInstance(params,uploaderInstance);
    }
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
    // This can run in a sep instance, so make sure state changes are sent to the right instance
    qDebug() << "Running in slot of Uploader class!";
    QString data = (QString) reply->readAll();
    qDebug() << data;
    UploadActionResult result;
    if (reply->error()){
        result.success = false;
        qDebug() << "network reply error!";

        // @TODO handle this better!
    }
    else {
        result.success = true;
        QJsonObject jsonResult = QJsonObject(QJsonDocument::fromJson(data.toUtf8()).object());
        // Parse the reply as JSON and map to result
        result.url = jsonResult["secure_url"].toString();
        result.id = jsonResult["public_id"].toString();
    }

    if (result.success){
        if (this->m_hasAttachedConfig){
            QString finalImageUrlToDownload = result.url;
            TransformationConfig config = this->m_attachedConfig;

            if (this->m_uploadInProgress==true){
                // We need to take the upload result and use for further processing
                // We should be able to just construct a URL string from the previously uploaded result, and the transformation part of the config
                QString computedFinalImageUrl = Cloudinary::generateImageUrlFromConfigAndId(result.id,config);
                finalImageUrlToDownload = computedFinalImageUrl;
                this->m_uploadInProgress = false;
            }

            else {
                // This was the last step - can finish up processing
                // Example of when this hits: Upload with a transformation config, but store_original = off - so image is directly uploaded with transformation
                // Just need to download... shouldn't need to do anything else here.
            }

            QFileInfo fileInfo = QFileInfo(this->m_localFilePath);
            QString pathToSaveFileTo = this->m_localFilePath;
            if (config.overwriteLocalFile == false){
                QString createdFileSuffix = config.createdFileSuffix!="" ? config.createdFileSuffix : "_dct";
                pathToSaveFileTo = fileInfo.absolutePath() + fileInfo.baseName() + createdFileSuffix;
                if (fileInfo.completeSuffix()!=""){
                    pathToSaveFileTo += "." + fileInfo.completeSuffix();
                }
            }

            // Actually download the file to disk
            Downloader::downloadImageFileToPath(finalImageUrlToDownload,pathToSaveFileTo);

            if (config.deleteCloudCopyAfterDownload){
                // @TODO
            }
        }
        else {
            // Copy url to clipboard
            QClipboard *clipboard = QGuiApplication::clipboard();
            clipboard->setText(result.url);
        }
    }

    // Push the result
    this->m_lastUploadActionResult = result;
    emit this->uploadActionResultReceived();
    // Finally, regardless of actual reply, close out the pending upload - this will also emit
    this->setUploadInProgress(false);
    Uploader::getInstance()->setUploadInProgress(false);
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
