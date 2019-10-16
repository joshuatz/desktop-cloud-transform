#include "downloader.h"
#include "logger.h"

Downloader *Downloader::m_instance = nullptr;

Downloader::Downloader(QObject *parent) : QObject(parent)
{

}

Downloader *Downloader::getInstance(){
    if (m_instance == nullptr){
        m_instance = new Downloader();
    }
    return m_instance;
}

void Downloader::downloadImageFileToPathWithSlotString(QString remotePath, QString localPath,QString id, QObject *receiver, QString slot){
    qDebug() << "Downloader function called with receiver and slot";
    QNetworkAccessManager *netManager = new QNetworkAccessManager();
    QNetworkRequest request(remotePath);
    // Set up connect()
    QObject::connect(netManager,&QNetworkAccessManager::finished,[=](QNetworkReply *finishedReply) {
        bool downloadWorked = true;
        if (finishedReply->error()){
            // Uh-Oh!
            qDebug() << "Downloading file failed! Remote URL = " << remotePath << " || Saving to : " << localPath;
            downloadWorked = false;
        }
        else {
            QFile *file = new QFile(localPath);
            if (file->open(QIODevice::WriteOnly)){
                file->write(finishedReply->readAll());
                file->flush();
                file->close();
                downloadWorked = true;
            }
            else {
                // Uh-Oh!
                QString errMsg = "Download success, but IO error writing to disc at path : " + localPath;
                qDebug() << errMsg;
                Logger().getInstance()->logStr(errMsg);
                downloadWorked = false;
            }
            delete file;
        }
        if (receiver!=nullptr && slot!=""){
            QMetaObject::invokeMethod(receiver,slot.toUtf8(),Qt::AutoConnection,Q_ARG(bool,downloadWorked));
        }
        emit Downloader::getInstance()->downloadFinished(downloadWorked);
        finishedReply->deleteLater();
    });
    // Make the request
    netManager->get(request);
}

void Downloader::downloadImageFileToPath(QString remotePath,QString localPath,QString id){
    Downloader::downloadImageFileToPathWithSlotString(remotePath,localPath,id,nullptr,"");
}
