#include "downloader.h"

Downloader *Downloader::m_instance = nullptr;

Downloader *Downloader::getInstance(){
    if (m_instance == nullptr){
        m_instance = new Downloader();
    }
    return m_instance;
}

//void Downloader::downloadImageFileToPath(QString remotePath, QString localPath, QObject *receiver, uploaderMemPtr slot)
//{
//    qDebug() << "Downloader called with special ptr to member of qobject - Uploader::*";
////    Downloader::downloadImageFileToPathWithSlot(remotePath,localPath,receiver,SLOT(slot));
//    Downloader::downloadImageFileToPathWithSlot(remotePath,localPath,receiver,slot);
//}

void Downloader::downloadImageFileToPathWithSlotString(QString remotePath, QString localPath, QObject *receiver, QString slot)
{
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
                qDebug() << "Download success, but IO error writing to disc at path : " << localPath;
                downloadWorked = false;
            }
            delete file;
        }
        if (receiver!=nullptr){


//            QMetaObject::invokeMethod(receiver,slot,Qt::AutoConnection,Q_RETURN_ARG(bool,downloadWorked));
//            QMetaObject::invokeMethod(receiver,slot,Qt::AutoConnection,Q_RETURN_ARG(bool,downloadWorked));
//            QMetaObject::invokeMethod(receiver,std::forward<Functor>(slot),Qt::AutoConnection,Q_RETURN_ARG(bool,downloadWorked));
//            std::invoke(slot,receiver,downloadWorked);
//            (receiver.*slot)(downloadWorked);
            QMetaObject::invokeMethod(receiver,slot.toUtf8(),Qt::AutoConnection,Q_ARG(bool,downloadWorked));
        }
        emit Downloader::getInstance()->downloadFinished(downloadWorked);
        finishedReply->deleteLater();
    });
    // Make the request
    netManager->get(request);
}

Downloader::Downloader(QObject *parent) : QObject(parent)
{

}

//void Downloader::downloadImageFileToPath(QString remotePath, QString localPath, QObject *receiver, const char *slot){
////    Downloader::downloadImageFileToPath(remotePath,localPath,[reciever](bool res){
////        //
////    });
//    QObject::connect()
//}

//void Downloader::downloadImageFileToPath(QString remotePath,QString localPath){
//    Downloader::downloadImageFileToPath(remotePath,localPath,nullptr,nullptr);
//}

//template <typename Func>
//void Downloader::downloadImageFileToPath(QString remotePath, QString localPath, QObject *receiver, Func slot){
//void Downloader::downloadImageFileToPath(QString remotePath, QString localPath, QObject *receiver, const char* slot){
void Downloader::downloadImageFileToPath(QString remotePath,QString localPath){
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
                qDebug() << "Download success, but IO error writing to disc at path : " << localPath;
                downloadWorked = false;
            }
            delete file;
        }
//        if (receiver!=nullptr){
//            QObject::connect(Downloader::getInstance(),&Downloader::downloadFinished,receiver,slot);
//        }
        emit Downloader::getInstance()->downloadFinished(downloadWorked);
        finishedReply->deleteLater();
    });
    // Make the request
    netManager->get(request);

}

//void Downloader::downloadImageFileToPath(QString remotePath,QString localPath,void(*fnPtr)(bool res)){
//    QNetworkAccessManager *netManager = new QNetworkAccessManager();
//    QNetworkRequest request(remotePath);
//    // Set up connect()
//    QObject::connect(netManager,&QNetworkAccessManager::finished,[=](QNetworkReply *finishedReply) {
//        bool downloadWorked = true;
//        if (finishedReply->error()){
//            // Uh-Oh!
//            qDebug() << "Downloading file failed! Remote URL = " << remotePath << " || Saving to : " << localPath;
//            downloadWorked = false;
//        }
//        else {
//            QFile *file = new QFile(localPath);
//            if (file->open(QIODevice::WriteOnly)){
//                file->write(finishedReply->readAll());
//                file->flush();
//                file->close();
//                downloadWorked = true;
//            }
//            else {
//                // Uh-Oh!
//                qDebug() << "Download success, but IO error writing to disc at path : " << localPath;
//                downloadWorked = false;
//            }
//            delete file;
//        }
////        if (receiver!=nullptr){
////            QObject::connect(Downloader::getInstance(),&Downloader::downloadFinished,receiver,slot);
////        }
//        fnPtr(downloadWorked);
//        emit Downloader::getInstance()->downloadFinished(downloadWorked);
//        finishedReply->deleteLater();
//    });
//    // Make the request
//    netManager->get(request);

//}

//void Downloader::downloadImageFileToPath(QString remotePath, QString localPath, std::function<Uploader* (bool)>fnPtr)
//{
//    QNetworkAccessManager *netManager = new QNetworkAccessManager();
//    QNetworkRequest request(remotePath);
//    // Set up connect()
//    QObject::connect(netManager,&QNetworkAccessManager::finished,[=](QNetworkReply *finishedReply) {
//        bool downloadWorked = true;
//        if (finishedReply->error()){
//            // Uh-Oh!
//            qDebug() << "Downloading file failed! Remote URL = " << remotePath << " || Saving to : " << localPath;
//            downloadWorked = false;
//        }
//        else {
//            QFile *file = new QFile(localPath);
//            if (file->open(QIODevice::WriteOnly)){
//                file->write(finishedReply->readAll());
//                file->flush();
//                file->close();
//                downloadWorked = true;
//            }
//            else {
//                // Uh-Oh!
//                qDebug() << "Download success, but IO error writing to disc at path : " << localPath;
//                downloadWorked = false;
//            }
//            delete file;
//        }
////        if (receiver!=nullptr){
////            QObject::connect(Downloader::getInstance(),&Downloader::downloadFinished,receiver,slot);
////        }
//        fnPtr(downloadWorked);
//        emit Downloader::getInstance()->downloadFinished(downloadWorked);
//        finishedReply->deleteLater();
//    });
//    // Make the request
//    netManager->get(request);
//}

template<typename Functor>
void Downloader::downloadImageFileToPathWithSlot(QString remotePath, QString localPath, QObject *receiver, Functor slot)
//void Downloader::downloadImageFileToPath(QString remotePath, QString localPath, QObject *receiver, Functor slot)
{
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
                qDebug() << "Download success, but IO error writing to disc at path : " << localPath;
                downloadWorked = false;
            }
            delete file;
        }
        if (receiver!=nullptr){

            QMetaObject::invokeMethod(receiver,slot,Qt::AutoConnection,Q_RETURN_ARG(bool,downloadWorked));
//            QMetaObject::invokeMethod(receiver,slot,Qt::AutoConnection,Q_RETURN_ARG(bool,downloadWorked));
//            QMetaObject::invokeMethod(receiver,std::forward<Functor>(slot),Qt::AutoConnection,Q_RETURN_ARG(bool,downloadWorked));
//            std::invoke(slot,receiver,downloadWorked);
//            (receiver.*slot)(downloadWorked);
        }
        emit Downloader::getInstance()->downloadFinished(downloadWorked);
        finishedReply->deleteLater();
    });
    // Make the request
    netManager->get(request);

}
