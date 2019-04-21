#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>
#include <QFile>
#include <QtNetwork>
#include "transformationconfig.h"

struct UploadActionResult {
    bool success;
    QString url;
    QString id;
    bool hasAttachedConfig = false;
    TransformationConfig attachedConfig;
    bool savedLocally = false;
    QString localSavePath = "";
    QString messageString = "";
    UploadActionResult(bool success = false,QString url="",QString id="") :
        success(success),
        url(url),
        id(id)
    {}
    // Need to use Q_GADGET so members can be understood across meta registration
    Q_GADGET
        Q_PROPERTY(bool success MEMBER success)
        Q_PROPERTY(QString url MEMBER url)
        Q_PROPERTY(QString id MEMBER id)
        Q_PROPERTY(bool hasAttachedConfig MEMBER hasAttachedConfig)
        Q_PROPERTY(TransformationConfig attachedConfig MEMBER attachedConfig)
        Q_PROPERTY(bool savedLocally MEMBER savedLocally)
        Q_PROPERTY(QString localSavePath MEMBER localSavePath)
        Q_PROPERTY(QString messageString MEMBER messageString)
};

class Uploader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool uploadInProgress MEMBER m_uploadInProgress NOTIFY uploadInProgressChanged)
    Q_PROPERTY(bool downloadInProgress MEMBER m_downloadInProgress NOTIFY downloadInProgressChanged)
    Q_PROPERTY(UploadActionResult lastUploadActionResult READ getLastUploadResult NOTIFY uploadActionResultReceived)
public:
    explicit Uploader(QObject *parent = nullptr);
    static Uploader *getInstance();
    UploadActionResult getLastUploadResult();
    bool getUploadInProgress();
    bool getDownloadInProgress();
    bool getActionChainInProgress();
    void setSuccessOfLastResult(bool success);
    void setMessageOfLastResult(QString message);
    void setActionChainInProgress(bool inProgress);
    void setDeletionInProgress(bool inProgress);
signals:
//    void queueChanged();
    void uploadInProgressChanged();
    void downloadInProgressChanged();
    void uploadActionResultReceived();
    void actionChainInProgressChanged();
    void deletionInProgressChanged();
public slots:
    int uploadImageFromLocalPath(QString localImageFilePath);
    void receiveNetworkReply(QNetworkReply *reply);
    void receiveDownloadResultSlot(bool res);
    void setUploadInProgress(bool uploadInProgressStatus);
    void setDownloadInProgress(bool updatedDownloadInProgress);
    void uploadImageWithConfig(QString localImageFilePath,TransformationConfig config);
    void uploadImageWithConfigId(QString localImageFilePath,int configId);
    static UploadActionResult mockUploadResult(QString type);
    static QString macroReplacer(QString inputString,QString localImageFilePath,QMap<QString,QString> OPT_additionalReplacements);
private:
    bool m_uploadInProgress = false;
    bool m_downloadInProgress = false;
    bool m_deletionInProgress = false;
    QList<QString> m_queuedDeletes;
    // Attached configs finish out when all the upload stuff and generation of final URLs has finished. They finish before downloads are done.
    bool m_attachedConfigIsInProgress = false;
    // The action chain is not done until absolutely everything is done - including downloading images
    bool m_actionChainInProgress = false;
    int m_processingIndex = 0;
    int m_processingQueueLength = 0;
    UploadActionResult m_lastUploadActionResult;
    static Uploader *m_instance;
    bool m_hasAttachedConfig = false;
    TransformationConfig m_attachedConfig;
    QString m_localFilePath;
};

Q_DECLARE_METATYPE(UploadActionResult);

#endif // UPLOADER_H
