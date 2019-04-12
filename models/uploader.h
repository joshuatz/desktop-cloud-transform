#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>
#include <QtNetwork>

class Uploader : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(bool uploadInProgress MEMBER m_uploadInProgress NOTIFY uploadInProgressChanged)
public:
    explicit Uploader(QObject *parent = nullptr);

signals:
//    void queueChanged();
//    void uploadInProgressChanged();
public slots:
    int uploadImageFromLocalPath(QString localImageFilePath);
    static void receiveNetworkReply(QNetworkReply *reply);
private:
    bool m_uploadInProgress = false;
    int m_processingIndex = 0;
    int m_processingQueueLength = 0;
};

#endif // UPLOADER_H
