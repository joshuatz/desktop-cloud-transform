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
};

class Uploader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool uploadInProgress MEMBER m_uploadInProgress NOTIFY uploadInProgressChanged)
    Q_PROPERTY(UploadActionResult lastUploadActionResult READ getLastUploadResult NOTIFY uploadActionResultReceived)
public:
    explicit Uploader(QObject *parent = nullptr);
    static Uploader *getInstance();
    UploadActionResult getLastUploadResult();
signals:
//    void queueChanged();
    void uploadInProgressChanged();
    void uploadActionResultReceived();
public slots:
    int uploadImageFromLocalPath(QString localImageFilePath);
    void receiveNetworkReply(QNetworkReply *reply);
    void setUploadInProgress(bool uploadInProgressStatus);
    void uploadImageWithConfig(QString localImageFilePath,TransformationConfig config);
    void uploadImageWithConfigId(QString localImageFilePath,int configId);
private:
    bool m_uploadInProgress = false;
    int m_processingIndex = 0;
    int m_processingQueueLength = 0;
    UploadActionResult m_lastUploadActionResult = UploadActionResult();
    static Uploader *m_instance;
    bool m_hasAttachedConfig = false;
    TransformationConfig m_attachedConfig;
    bool m_attachedConfigIsInProgress = false;
    QString m_localFilePath;
};

Q_DECLARE_METATYPE(UploadActionResult);

#endif // UPLOADER_H
