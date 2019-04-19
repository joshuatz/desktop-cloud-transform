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
#include <QRegularExpression>
#include <QImageReader>

Uploader::Uploader(QObject *parent) : QObject(parent)
{
}

Uploader *Uploader::m_instance = nullptr;

Uploader *Uploader::getInstance(){
    if (m_instance == nullptr){
        m_instance = new Uploader();
//        QObject::connect(Downloader::getInstance(),&Downloader::downloadFinished,m_instance,&Uploader::receiveDownloadResult);
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

QString Uploader::macroReplacer(QString inputString, QString localImageFilePath, QMap<QString, QString> OPT_additionalReplacements){
    QString finalString = inputString;
    QRegularExpression validMacroPattern = QRegularExpression("{[^}]+}");

    QMap<QString,QString> replacerMap = OPT_additionalReplacements;

    // Get image file details
    QImageReader reader(localImageFilePath);
    if (reader.canRead()){
        QSize sizeDetails = reader.size();
        replacerMap.insert("{width}",QString::number(sizeDetails.width()));
        replacerMap.insert("{height}",QString::number(sizeDetails.height()));
    }

    // Get regular file details
    QFileInfo fileInfo(localImageFilePath);
    if (fileInfo.exists()){
        replacerMap.insert("{filename}",fileInfo.fileName());
        replacerMap.insert("{filenameNoExt}",fileInfo.baseName());
        replacerMap.insert("{basename}",fileInfo.baseName());
        replacerMap.insert("{created_epochs}",QString::number(fileInfo.birthTime().toSecsSinceEpoch()));
    }

    QMap<QString,QString>::iterator i;
    for (i = replacerMap.begin(); i != replacerMap.end(); ++i){
        QString macro = i.key();
        QString macroVal = i.value();
        if (validMacroPattern.match(macro).hasMatch()==false){
            macro = "{" + macro + "}";
        }
        finalString = finalString.replace(macro,macroVal,Qt::CaseInsensitive);
    }
    return finalString;
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

    // Used to indicate that the image that was just uploaded was "raw" without a config applied
    bool uploadWasRawFlag = false;
    // Flag to indicate that a download is going to be triggered that will delay the "success" of the current chain of events
    bool waitingOnDownload = false;

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
        result.messageString = "Error in response from Cloudinary";
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
                uploadWasRawFlag = true;
                // We need to take the upload result and use for further processing
                // Example: store_original = on, so image was just uploaded as-is and config has yet to be applied
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

            if (config.usesOutgoingTransformationRawString){
                // Currently, it only makes sense to use this if the outgoing transformation string references the uploaded image.
                if (config.outoingTransformationRawString.contains("{uploaded}",Qt::CaseInsensitive)){
                    QString uploadImageMacroVal = "";
                    // We need to construct a NEW final image url
                    // Basically need to compute string to represent the BASE (original raw image) + any incoming trans (raw trans string, named preset, etc)
                    // Then substitute macros in the outgoing trans string with that var
                    if (uploadWasRawFlag || this->m_attachedConfigIsInProgress){
                        // If the original image was uploaded without any trans applied, we could either upload again with the same image + trans, or cheat and represent the combo of base + trans as a remote image layer (base64 encoded)
                        // Represented as "l_fetch:base64websafeurl"
                        uploadImageMacroVal = "l_fetch:" + finalImageUrlToDownload.toUtf8().toBase64(QByteArray::Base64UrlEncoding);
                    }
                    else {
                        // If the image that was just uploaded already has all the incoming trans applied it needed, we can simply reference the image as is
                        // To use a cloudinary public image by id - "l_[PUBLICID]"
                        uploadImageMacroVal = "l_" + result.id;
                    }
                    QString finalOutgoingTransString = config.outoingTransformationRawString.replace("{uploaded}",uploadImageMacroVal,Qt::CaseSensitivity::CaseInsensitive);
                    // Final macro replacements
                    finalOutgoingTransString = Uploader::macroReplacer(finalOutgoingTransString,this->m_localFilePath,QMap<QString,QString>());
                    // Set the final fetch url to the cloudinary upload base URL + computed trans string, and count as ANOTHER trans credit use
                    Stats::getInstance()->logStat("cloudinary","transform",true);
                    finalImageUrlToDownload = Helpers::forceEndingSlash(Cloudinary::getPublicUploadUrlBase()) + Helpers::removeBeginSlash(finalOutgoingTransString);
                    result.url = finalImageUrlToDownload;
                }
            }

            if (config.saveLocally){
                // Set flag to block success signal until download complete
                waitingOnDownload = true;

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

                Downloader::downloadImageFileToPath(finalImageUrlToDownload,pathToSaveFileTo,this,&Uploader::receiveDownloadResult);
//                Downloader::downloadImageFileToPath(finalImageUrlToDownload,pathToSaveFileTo);

//                Downloader::downloadImageFileToPath(finalImageUrlToDownload,pathToSaveFileTo,[this](bool res){

//                });

                Stats::getInstance()->logStat("application","download",false);
                Stats::getInstance()->logStat("cloudinary","download",true);

                // Update result
                result.savedLocally = true;
                result.localSavePath = pathToSaveFileTo;
                // This success will be changed in the callback to download - depending on if download succeeds
                result.success = false;

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
    Uploader::getInstance()->setUploadInProgress(false);
    if (waitingOnDownload==false){
        emit Uploader::getInstance()->uploadActionResultReceived();
    }
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

void Uploader::receiveDownloadResult(bool res){
    qDebug() << "Running in slot receiveDownloadResult!";
    qDebug() << res;
    Uploader::getInstance()->setSuccessOfLastResult(res);
    if (!res){
        this->setMessageOfLastResult("Download failed!");
    }
    if(this->getUploadInProgress()){
        // finish out chain and emit event
        this->setUploadInProgress(false);
        emit Uploader::getInstance()->uploadActionResultReceived();
    }
    // Regardless of success, set downloading flag to false
    this->setDownloadInProgress(false);
}

bool Uploader::getUploadInProgress(){
    return m_uploadInProgress;
}

bool Uploader::getDownloadInProgress(){
    return m_downloadInProgress;
}

void Uploader::setDownloadInProgress(bool updatedDownloadInProgress){
    if (m_downloadInProgress!=updatedDownloadInProgress){
        m_downloadInProgress = updatedDownloadInProgress;
        emit Uploader::getInstance()->downloadInProgressChanged();
    }
}

void Uploader::setSuccessOfLastResult(bool success){
    this->m_lastUploadActionResult.success = success;
}

void Uploader::setMessageOfLastResult(QString message){
    this->m_lastUploadActionResult.messageString = message;
}
