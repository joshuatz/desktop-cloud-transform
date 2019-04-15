#include "transformationlist.h"
#include "database.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

TransformationList *TransformationList::m_instance = nullptr;

TransformationList *TransformationList::getInstance(){
    if (m_instance == nullptr){
        m_instance = new TransformationList();
    }
    return m_instance;
}
QMap<int,TransformationConfig> TransformationList::configMapById = QMap<int,TransformationConfig>();
QMap<int,QVariant> TransformationList::configVariantMapById = QMap<int,QVariant>();
const QString TransformationList::TABLENAME = "transformationconfigs";

TransformationList::TransformationList(QObject *parent) : QObject(parent)
{
}

TransformationList::ConfigList TransformationList::getDefaults(){
    // @TODO
    TransformationList::ConfigList defaults;
    // Twitter crop
    TransformationConfig twitterConfig;

    return defaults;
}

void TransformationList::loadAllFromStorage(){
    if (Database::connected){
        QSqlQueryModel queryModel;
        queryModel.setQuery(QStringList({"SELECT * FROM ",TransformationList::TABLENAME}).join(""));
        for (int i=0; i < queryModel.rowCount(); ++i){
            TransformationConfig currConfig = TransformationList::sqlRowToTransformationConfig(queryModel.record(i));
            this->configMapById.insert(currConfig.id,currConfig);
            this->configVariantMapById.insert(currConfig.id,QVariant::fromValue(currConfig));
        }
        qDebug() << "Configs loaded from storage. Count = " << this->configMapById.count();
        // @TODO better checking to see if records have actually changed vs stayed the same
        emit this->transformationsChanged();
    }
    else {
        qDebug() << "Fatal error! DB disconnected!";
    }
}

void TransformationList::saveAllToStorage(){
    // @TODO
}

int TransformationList::saveNewToStorage(TransformationConfig newConfig){
    int insertedRecordId = -1;
    bool res = false;
    if (Database::connected){
        QSqlQuery insertQuery;
        insertQuery.prepare(QStringList({"INSERT INTO ",TransformationList::TABLENAME," (user_defined_name,overwrite_local,created_file_suffix,uses_preset,preset_name,uses_trans_string,trans_string,store_original,delete_cloud_after_download) VALUES(:user_defined_name,:overwrite_local,:created_file_suffix,:uses_preset,:preset_name,;uses_trans_string,:trans_string,:store_original,:delete_cloud_after_download)"}).join(""));
        insertQuery.bindValue(":user_defined_name",newConfig.userDefinedName);
        insertQuery.bindValue(":overwrite_local",newConfig.overwriteLocalFile);
        insertQuery.bindValue(":created_file_suffix",newConfig.createdFileSuffix);
        insertQuery.bindValue(":uses_preset",newConfig.usesPreset);
        insertQuery.bindValue(":preset_name",newConfig.presetName);
        insertQuery.bindValue(":uses_trans_string",newConfig.usesTransformationRawString);
        insertQuery.bindValue(":trans_string",newConfig.transformationRawString);
        insertQuery.bindValue(":store_original",newConfig.storeOriginal);
        insertQuery.bindValue(":delete_cloud_after_download",newConfig.deleteCloudCopyAfterDownload);
        res = insertQuery.exec();
        if (res){
            insertedRecordId = insertQuery.lastInsertId().Int;
        }
    }
    return insertedRecordId;
}

void TransformationList::updateExistingInStorage(){
    // @TODO
}

TransformationConfig TransformationList::sqlRowToTransformationConfig(QSqlRecord row){
    TransformationConfig result;
    result.id = row.value("id").toInt();
    result.userDefinedName = row.value("user_defined_name").toString();
    result.overwriteLocalFile = row.value("overwrite_local").toBool();
    result.createdFileSuffix = row.value("created_file_suffix").toString();
    result.usesPreset = row.value("uses_preset").toBool();
    result.presetName = row.value("preset_name").toString();
    result.usesTransformationRawString = row.value("uses_trans_string").toBool();
    result.transformationRawString = row.value("trans_string").toString();
    result.storeOriginal = row.value("store_original").toBool();
    result.deleteCloudCopyAfterDownload = row.value("delete_cloud_after_download").toBool();
    return result;
}

QVariantList TransformationList::getConfigListAsVariantList(){
    return TransformationList::configVariantMapById.values();
}

TransformationConfig TransformationList::getConfigById(int id){
    TransformationConfig config;
    if (Database::connected){
        QSqlQueryModel queryModel;
        queryModel.setQuery(QStringList({"SELECT * FROM ",TransformationList::TABLENAME," WHERE ",TransformationList::TABLENAME,".id = ",QString::number(id)}).join(""));
        if (queryModel.rowCount() > 0){
            QSqlRecord currRecord = queryModel.record(0);
            config = TransformationList::sqlRowToTransformationConfig(currRecord);
        }
    }
    return config;
}

QMap<QString,QVariant> TransformationList::configToParams(TransformationConfig config){
    // @TODO
    QMap<QString,QVariant> params;
    if (config.usesPreset){
        // If using a named preset, ignore manual trans string
        params.insert("upload_preset",QVariant(config.presetName));
    }
    else if (config.usesTransformationRawString) {
        //
    }
    return params;
}
