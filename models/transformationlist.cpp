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
    return TransformationList::insertOrUpdateInStorage(newConfig,true);
}

void TransformationList::updateExistingInStorage(){
    // @TODO
}

int TransformationList::insertOrUpdateInStorage(TransformationConfig config, bool isUpdate){
    int insertedRecordId = -1;
    bool res = false;
    if (Database::connected){
        QSqlQuery insertQuery;
        // Construct SQL query dynamically
        QString query = "REPLACE INTO " + TransformationList::TABLENAME + " (" + (isUpdate ? "id," : "") + "user_defined_name,save_locally,overwrite_local,created_file_suffix,uses_preset,preset_name,uses_named_trans,named_trans,uses_trans_string,trans_string,store_original,delete_cloud_after_download) VALUES(" + (isUpdate ? ":id," : "") + ":user_defined_name,:save_locally,:overwrite_local,:created_file_suffix,:uses_preset,:preset_name,:uses_named_trans,:named_trans,:uses_trans_string,:trans_string,:store_original,:delete_cloud_after_download)";
        insertQuery.prepare(query);
        if (isUpdate){
            insertQuery.bindValue(":id",config.id);
        }
        insertQuery.bindValue(":user_defined_name",config.userDefinedName);
        insertQuery.bindValue(":save_locally",config.saveLocally);
        insertQuery.bindValue(":overwrite_local",config.overwriteLocalFile);
        insertQuery.bindValue(":created_file_suffix",config.createdFileSuffix);
        insertQuery.bindValue(":uses_preset",config.usesPreset);
        insertQuery.bindValue(":preset_name",config.presetName);
        insertQuery.bindValue(":uses_named_trans",config.usesNamedTransformation);
        insertQuery.bindValue(":named_trans",config.namedTransformation);
        insertQuery.bindValue(":uses_trans_string",config.usesTransformationRawString);
        insertQuery.bindValue(":trans_string",config.transformationRawString);
        insertQuery.bindValue(":store_original",config.storeOriginal);
        insertQuery.bindValue(":delete_cloud_after_download",config.deleteCloudCopyAfterDownload);
        res = insertQuery.exec();
        if (res){
            insertedRecordId = insertQuery.lastInsertId().Int;
        }
    }
    return insertedRecordId;
}

TransformationConfig TransformationList::sqlRowToTransformationConfig(QSqlRecord row){
    TransformationConfig result;
    result.id = row.value("id").toInt();
    result.userDefinedName = row.value("user_defined_name").toString();
    result.saveLocally = row.value("save_locally").toBool();
    result.overwriteLocalFile = row.value("overwrite_local").toBool();
    result.createdFileSuffix = row.value("created_file_suffix").toString();
    result.usesPreset = row.value("uses_preset").toBool();
    result.presetName = row.value("preset_name").toString();
    result.usesNamedTransformation = row.value("uses_named_trans").toBool();
    result.namedTransformation = row.value("named_trans").toString();
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

bool TransformationList::deleteConfigByid(int configId){
    bool success = false;

    // @TODO

    return success;
}
