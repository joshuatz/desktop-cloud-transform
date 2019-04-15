#include "helpers.h"
#include <QObject>
#include <QDebug>
#include <QImageReader>
#include <QtNetwork>
#include <QGuiApplication>
#include <QClipboard>

Helpers::Helpers(QObject *parent) : QObject(parent)
{

}

void Helpers::checkInternetConnection(bool *res){

    qDebug() << "Checking internet connection...";
    QNetworkAccessManager *netManager = new QNetworkAccessManager();

    // Use lambda as receiver with connect to get result of network finished event
    QObject::connect(netManager,&QNetworkAccessManager::finished,[=](QNetworkReply *reply) {
        if (reply->error()){
            *res = false;
        }
        else {
            *res = true;
        }
    });

    // Start the network request
    QNetworkReply *netReply = netManager->get(QNetworkRequest(QString("https://www.google.com/")));
}

bool Helpers::checkValidImageFilePath(QString imagePath){
    // @TODO combine with additonal check of MIME type and generic can-access-file check.
    // Currently failing if image extension != detected mime type
    QImageReader reader(imagePath);
    if (reader.canRead()){
        return true;
    }
    return false;
}

QUrlQuery Helpers::generateUrlQueryFromVarMap(QMap<QString, QVariant> params){
    QUrlQuery query;
    QMap<QString,QVariant>::iterator i;
    for (i = params.begin(); i != params.end(); ++i){
        query.addQueryItem(i.key(),i.value().toString());
    }
    return query;
}

void Helpers::copyToClipboard(QString strToCopy){
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(strToCopy);
}
