#include "database.h"
#include "stats.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

const QString Stats::TABLENAME = "stats";
Stats *Stats::m_instance = nullptr;

Stats *Stats::getInstance(){
    if(m_instance==nullptr){
        m_instance = new Stats();
    }
    return m_instance;
}



Stats::Stats(QObject *parent) : QObject(parent)
{

}

void Stats::forceRefreshFromDb(){
    // @TODO
}

int Stats::logStat(QString category, QString action, bool usedCloudinary){
    return logStatWithConfig(category,action,usedCloudinary,false,NULL);
}

int Stats::logStatWithConfig(QString category, QString action, bool usedCloudinary, bool usedConfig, int configId){
    int insertedRecordId = -1;
    bool res = false;
    // Create timestamp int
    long long timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    if (Database::connected){
        QSqlQuery insertQuery;
        QString query = "INSERT INTO " + Stats::TABLENAME + " (stamp,category,action,used_cloudinary" + (usedConfig ? ",transformation_config" : "") + ") VALUES(:stamp,:category,:action,:used_cloudinary" + (usedConfig ? ",:transformation_config" : "") + ")";
        insertQuery.prepare(query);
        insertQuery.bindValue(":stamp",timestamp);
        insertQuery.bindValue(":category",category);
        insertQuery.bindValue(":action",action);
        insertQuery.bindValue(":used_cloudinary",usedCloudinary);
        if (usedConfig){
            insertQuery.bindValue(":transformation_config",configId);
        }
        res = insertQuery.exec();
        if (res){
            insertedRecordId = insertQuery.lastInsertId().toInt();
        }
    }
    emit Stats::getInstance()->statsUpdated();
    return insertedRecordId;
}
