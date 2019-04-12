#include "uploader.h"
#include "helpers.h"

Uploader::Uploader(QObject *parent) : QObject(parent)
{

}

int Uploader::uploadImageFromLocalPath(QString localImageFilePath){
    // @TODO implement queue system
    int queueNumber = 0;
    if (Helpers::checkValidImageFilePath(localImageFilePath)){
        this->m_processingIndex = queueNumber;
    }
    return queueNumber;
}
