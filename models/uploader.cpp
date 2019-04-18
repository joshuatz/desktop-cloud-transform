#include "uploader.h"
#include "helpers.h"
#include "apis/cloudinary.h"
#include "downloader.h"
#include "stats.h"
#include "transformationlist.h"
#include <QClipboard>
#include <QFile>
#include <QFileInfo>
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

void Uploader::uploadImageWithConfigId(QString localImageFilePath,int configId){
    TransformationConfig config = TransformationList::getConfigById(configId);
    this->uploadImageWithConfig(localImageFilePath,config);
}

UploadActionResult Uploader::mockUploadResult(QString type){
    UploadActionResult result;
    result.success = type!="fail";
    result.id = "Pfj1C029Cj";
    if (type=="withConfigSaveLocal" || type=="withConfig"){
        result.hasAttachedConfig = true;
        result.attachedConfig = TransformationConfig();
        if (type=="withConfigSaveLocal"){
            result.attachedConfig.saveLocally = true;
            result.localSavePath = "C:/Users/Joshua/Downloads/Temp/TRUETEMP/tmb9BkG.jpg";
            result.url = "https://cloudinary-res.cloudinary.com/image/upload/c_scale,w_200/v1538584137/cloudinary_logo_for_black_bg.png";
        }
        else {
            result.attachedConfig.saveLocally = false;
            result.url = "https://cloudinary-res.cloudinary.com/image/upload/c_scale,w_200/v1538584137/cloudinary_logo_for_black_bg.png";
        }
    }
    return result;

}

void Uploader::uploadImageWithConfig(QString localImageFilePath,TransformationConfig config){
    Stats::getInstance()->logStat("cloudinary","upload",true);
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
        // This is the final upload action - apply transformation WITH upload (so only transformed version is stored in cloud)
        Stats::getInstance()->logStat("cloudinary","transform",true);
        if (config.usesPreset){
            params.insert("upload_preset",QVariant(config.presetName));
        }
        else if (config.usesNamedTransformation){
            // Named transformations use normal chain syntax, but just insert as "t_NAMEOFTRANSFORMATION"
            QString transString = "t_" + config.namedTransformation;
            params.insert("transformation",QVariant(transString));
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
        Stats::getInstance()->logStat("cloudinary","upload",true);
        Uploader::getInstance()->setUploadInProgress(true);
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
    result.hasAttachedConfig = this->m_hasAttachedConfig;
    if (result.hasAttachedConfig){
        result.attachedConfig = this->m_attachedConfig;
    }

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

            if (this->m_attachedConfigIsInProgress==true){
                // We need to take the upload result and use for further processing
                Stats::getInstance()->logStat("cloudinary","transform",true);
                // We should be able to just construct a URL string from the previously uploaded result, and the transformation part of the config
                QString computedFinalImageUrl = Cloudinary::generateImageUrlFromConfigAndId(result.id,config);
                finalImageUrlToDownload = computedFinalImageUrl;
                // make sure to set the final URL to the computed public image url, not the base image url
                result.url = computedFinalImageUrl;
                // Set progress flag
                this->m_attachedConfigIsInProgress = false;
            }

            else {
                // This was the last step - can finish up processing
                // Example of when this hits: Upload with a transformation config, but store_original = off - so image is directly uploaded with transformation
                // Just need to download... shouldn't need to do anything else here.
            }
            if (config.saveLocally){
                QFileInfo fileInfo = QFileInfo(this->m_localFilePath);
                QString pathToSaveFileTo = this->m_localFilePath;

                if (config.overwriteLocalFile == false){
                    QString createdFileSuffix = config.createdFileSuffix!="" ? config.createdFileSuffix : "_dct";
                    pathToSaveFileTo = fileInfo.absolutePath() + "/" + fileInfo.baseName() + createdFileSuffix;
                    if (fileInfo.completeSuffix()!=""){
                        pathToSaveFileTo += "." + fileInfo.completeSuffix();
                    }
                }

                // Actually download the file to disk
                qDebug () << "Saving " << finalImageUrlToDownload << "  to  " << pathToSaveFileTo;
                Downloader::downloadImageFileToPath(finalImageUrlToDownload,pathToSaveFileTo);
                Stats::getInstance()->logStat("application","download",false);
                Stats::getInstance()->logStat("cloudinary","download",true);

                // Update result
                // Update result
                result.savedLocally = true;
                result.localSavePath = pathToSaveFileTo;

                if (config.deleteCloudCopyAfterDownload){
                    // @TODO
                }
            }
        }
        else {
            // Copy url to clipboard
            QClipboard *clipboard = QGuiApplication::clipboard();
            clipboard->setText(result.url);
        }
    }

    // Push the result - delay to allow file saving to finish out
    this->m_lastUploadActionResult = result;
    Uploader::getInstance()->m_lastUploadActionResult = result;
    // Finally, regardless of actual reply, close out the pending upload - this will also emit
    this->setUploadInProgress(false);
    QTimer::singleShot(400,[](){
        Uploader::getInstance()->setUploadInProgress(false);
        emit Uploader::getInstance()->uploadActionResultReceived();
    });
}

void Uploader::setUploadInProgress(bool uploadInProgressStatus){
    if (m_uploadInProgress != uploadInProgressStatus){
        m_uploadInProgress = uploadInProgressStatus;
        emit Uploader::getInstance()->uploadInProgressChanged();
    }
}


UploadActionResult Uploader::getLastUploadResult(){
    return m_lastUploadActionResult;
}
