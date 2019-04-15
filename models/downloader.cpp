#include "downloader.h"

Downloader::Downloader(QObject *parent) : QObject(parent)
{

}

void Downloader::downloadImageFileToPath(QString remotePath, QString localPath){
    // @TODO
    QNetworkAccessManager *netManager = new QNetworkAccessManager();
    QNetworkRequest request(remotePath);
    // Set up connect()
    QObject::connect(netManager,&QNetworkAccessManager::finished,[=](QNetworkReply *finishedReply) {
        if (finishedReply->error()){
            // Uh-Oh!
        }
        else {
            QFile *file = new QFile(localPath);
            if (file->open(QFile::Append)){
                file->write(finishedReply->readAll());
                file->flush();
                file->close();
            }
            delete file;
        }
        finishedReply->deleteLater();
    });
    // Make the request
    netManager->get(request);

}
