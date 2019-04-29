#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QtNetwork>
#include "uploader.h"

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
    static Downloader *getInstance();
    static void downloadImageFileToPathWithSlotString(QString remotePath,QString localPath,QString id,QObject *receiver,QString slot);
signals:
    void downloadFinished(bool res);
public slots:
    static void downloadImageFileToPath(QString remotePath,QString localPath,QString Id);
private:
    static Downloader *m_instance;
};

#endif // DOWNLOADER_H
