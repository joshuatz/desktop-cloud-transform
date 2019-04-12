#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>
#include <QtNetwork>

struct UploadActionResult {
    bool success;
    QString url;
    QString id;
    UploadActionResult(bool success = false,QString url="",QString id="") :
        success(success),
        url(url),
        id(id)
    {}
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
private:
    bool m_uploadInProgress = false;
    int m_processingIndex = 0;
    int m_processingQueueLength = 0;
    UploadActionResult m_lastUploadActionResult;
    static Uploader *m_instance;
};

Q_DECLARE_METATYPE(UploadActionResult);

#endif // UPLOADER_H
