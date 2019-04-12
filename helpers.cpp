#include "helpers.h"
#include <QObject>
#include <QDebug>

Helpers::Helpers()
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
