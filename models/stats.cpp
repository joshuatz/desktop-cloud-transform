#include "database.h"
#include "globalsettings.h"
#include "stats.h"
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>

const QString Stats::TABLENAME = "stats";
Stats *Stats::m_instance = nullptr;
GAnalytics *Stats::m_gaTrackerInstance = nullptr;
int Stats::m_cloudinaryUploadCount =  0;
int Stats::m_cloudinaryDownloadCount = 0;

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
    if (Database::connected){
        m_cloudinaryUploadCount = Stats::getCountOfStatAllTime("cloudinary","upload",true,-1);
        m_cloudinaryDownloadCount = Stats::getCountOfStatAllTime("cloudinary","download",true,-1);
        emit Stats::getInstance()->statsUpdated();
    }
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
    // Check if stat being logged is one that affects a stat stored in memory
    if (category=="cloudinary" && action=="upload"){
        m_cloudinaryUploadCount++;
    }
    else if (category=="cloudinary" && action=="download"){
        m_cloudinaryDownloadCount++;
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

int Stats::getCountOfStatOverDuration(QDateTime startDate, QDateTime endDate, QString category, QString action, bool usedCloudinary, int configId){
    int count = 0;
    bool res = false;
    if (Database::connected){
        QSqlQueryModel countQuery;
        // Start constructing query
        QString query = "SELECT count(id) AS count FROM " + Stats::TABLENAME + " WHERE (";
        query += "stamp < " + QString::number(endDate.toSecsSinceEpoch()) + " AND stamp > " + QString::number(startDate.toSecsSinceEpoch());
        if (category!="" && category!=nullptr){
            query+= " AND category = '" + category + "'";
        }
        if (action!="" && action!=nullptr){
            query+= " AND action = '" + action + "'";
        }
        if (usedCloudinary!=NULL){
            query+= " AND used_cloudinary = " + QString::number(usedCloudinary ? 1 : 0);
        }
        if (configId!=NULL && configId > 0){
            query+= " AND transformation_config = " + QString::number(configId);
        }
        query += ")";
        countQuery.setQuery(query);
        // There should only be one row of results, which contains the aggregated count based on the filter criteria
        if (countQuery.rowCount()==1){
            res = true;
            count = countQuery.record(0).value("count").toInt();
        }
        else {
            qDebug() << query;
            qDebug() << "Getting aggregate stat counts failed";
        }
    }
    return count;
}

int Stats::getCountOfStatAllTime(QString category, QString action, bool usedCloudinary, int configId){
    // Rather than duplicate the effort of the query that includes duration, I'm just going to fake a duration by picking dates before this app was ever created, and then 100 years into the future
    QDateTime startDate = QDateTime::fromSecsSinceEpoch(1524034172);
    QDateTime endDate = QDateTime::currentDateTime().addYears(100);
    // return from other func
    return Stats::getCountOfStatOverDuration(startDate,endDate,category,action,usedCloudinary,configId);
}
