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
    template <typename Functor>
    static void downloadImageFileToPathWithSlot(QString remotePath,QString localPath,QObject *receiver,Functor slot);
//    typedef std::function<void(Uploader::*)(bool)> uploaderMemPtr;
//    typedef void (Uploader::*)(bool) uploaderMemPtr;

    // Either of these work
    using uploaderMemPtr = void (Uploader::*)(bool);
//    typedef void (Uploader::*uploaderMemPtr)(bool);

//    static void downloadImageFileToPath(QString remotePath,QString localPath,QObject *receiver,UploaderDownloadReceivedPtr slot);
    static void downloadImageFileToPath(QString remotePath,QString localPath,QObject *receiver,uploaderMemPtr slot);
signals:
    void downloadFinished(bool res);
public slots:
    static void downloadImageFileToPath(QString remotePath,QString localPath);
//    static void downloadImageFileToPath(QString remotePath,QString localPath,void(*fnPtr)(bool res));
//    static void downloadImageFileToPath(QString remotePath,QString localPath,std::function<Uploader* (bool)>fnPtr);
//    static void downloadImageFileToPath(QString remotePath,QString localPath,std::function<bool (Uploader)>);
//    static void downloadImageFileToPath(QString remotePath,QString localPath,QObject *receiver,const char* slot);
//    static void downloadImageFileToPath(QString remotePath,QString localPath,QObject *receiver,QString methodName);
//    static void downloadImageFileToPath(QString remotePath,QString localPath,QObject *receiver,char* slot);

private:
    static Downloader *m_instance;
};

#endif // DOWNLOADER_H
