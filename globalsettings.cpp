#include "globalsettings.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <logger.h>

GlobalSettings::GlobalSettings(QObject *parent) : QObject(parent)
{

}

GlobalSettings *GlobalSettings::m_instance = nullptr;

GlobalSettings *GlobalSettings::getInstance(){
    if (m_instance == nullptr){
        m_instance = new GlobalSettings();
    }
    return m_instance;
}

QString GlobalSettings::TABLE_NAME = "globalsettings";

bool GlobalSettings::updateInBulk(QString cloudinaryCloudName, QString cloudinaryApiKey, QString cloudinaryApiSecret){
    // @TODO refactor and call other validate function
    bool validated = true;
    QList<QString> emptyStringCheck = {cloudinaryCloudName,cloudinaryApiKey,cloudinaryApiSecret};
    // Check for empty strings
    for (int x=0; x<emptyStringCheck.length(); x++){
        if (emptyStringCheck[x]==""){
            validated = false;
            break;
        }
    }
    if (validated){
        m_cloudinaryProperlyConfigured = true;
        m_cloudinaryCloudName = cloudinaryCloudName;
        m_cloudinaryApiKey = cloudinaryApiKey;
        m_cloudinaryApiSecret = cloudinaryApiSecret;
    }
    else {
        m_cloudinaryProperlyConfigured = false;
    }
    return validated;
}

void GlobalSettings::loadFromStorage(){
    m_cloudinaryCloudName = getSettingFromDb("cloudinaryCloudName").value.toString();
    m_cloudinaryApiKey = getSettingFromDb("cloudinaryApiKey").value.toString();
    m_cloudinaryApiSecret = getSettingFromDb("cloudinaryApiSecret").value.toString();
    m_optedOutTracking = getSettingFromDb("opted_out_tracking").value.toBool();
    m_userDebugLogOn = getSettingFromDb("user_debuglog_on").value.toBool();
    m_cloudinaryProperlyConfigured = this->validateCloudinarySettings();
    this->emitSettingsChanged();
}

void GlobalSettings::saveToStorage(){
    saveSettingToDb("cloudinaryCloudName",this->m_cloudinaryCloudName);
    saveSettingToDb("cloudinaryApiKey",this->m_cloudinaryApiKey);
    saveSettingToDb("cloudinaryApiSecret",this->m_cloudinaryApiSecret);
    saveSettingToDb("opted_out_tracking",this->m_optedOutTracking);
    saveSettingToDb("user_debuglog_on",this->m_userDebugLogOn);
    Logger().refreshLogPrefs();
    Logger().logStr("db updated");
}

settingDbResult GlobalSettings::getSettingFromDb(QString settingKey){
    QString settingValue = "";
    bool found = false;
    if (Database::connected){
        QSqlQuery query;
        QSqlQueryModel queryModel;
        queryModel.setQuery(QSqlQuery(QStringList({"SELECT setting_key,setting_value FROM ",GlobalSettings::TABLE_NAME," WHERE ",GlobalSettings::TABLE_NAME,".setting_key = '",settingKey,"'"}).join("")));
        settingValue = queryModel.rowCount() > 0 ? queryModel.record(0).value("setting_value").toString() : "";
        found = settingValue!="";
    }
    return settingDbResult(found,settingValue);
}

bool GlobalSettings::saveSettingToDb(QString settingKey, QVariant settingVal){
    bool res = false;
    if (Database::connected){
        QSqlQuery query;
        query.prepare(QStringList({"REPLACE INTO ",GlobalSettings::TABLE_NAME," (setting_key,setting_value) VALUES (:settingsKey, :settingVal);"}).join(""));
        query.bindValue(":settingsKey",settingKey);
        query.bindValue(":settingVal",settingVal);
        res = query.exec();
    }
    return res;
}

bool GlobalSettings::validateCloudinarySettings(){
    bool validated = true;
    QList<QString> emptyStringCheck = {this->m_cloudinaryCloudName,this->m_cloudinaryApiKey,this->m_cloudinaryApiSecret};
    // Check for empty strings
    for (int x=0; x<emptyStringCheck.length(); x++){
        if (emptyStringCheck[x]==""){
            validated = false;
            break;
        }
    }
    return validated;
}
