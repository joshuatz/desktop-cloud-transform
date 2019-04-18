#include "database.h"
#include "globalsettings.h"
#include "stats.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

const QString Stats::TABLENAME = "stats";
Stats *Stats::m_instance = nullptr;
GAnalytics *Stats::m_gaTrackerInstance = nullptr;

Stats *Stats::getInstance(){
    if(m_instance==nullptr){
        m_instance = new Stats();
    }
    return m_instance;
}

GAnalytics *Stats::getGaTracker(){
    #ifdef GAUID
        if(m_gaTrackerInstance==nullptr){
            m_gaTrackerInstance = new GAnalytics(QVariant(GAUID).toString());
        }
    #endif
    return m_gaTrackerInstance;
}


Stats::Stats(QObject *parent) : QObject(parent){
#ifdef GAUID
#endif
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

void Stats::fireGaEvent(QString category, QString action, QString label, QVariant value){
    #ifdef GAUID
        if (value==NULL){
            value = QVariant();
        }
        if(GlobalSettings::getInstance()->getIsOptedOutTracking()==false){
            Stats::getInstance()->getGaTracker()->sendEvent(category,action,label,value);
        }
    #endif
}
