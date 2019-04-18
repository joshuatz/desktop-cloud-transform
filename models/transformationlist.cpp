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

/**
 * @brief TransformationList::loadAllFromStorage - Loads all configs that are stored in the DB
 *
 * This updates BOTH the configMapById and configVariantMapById, which are static QMaps
 * It also emits a signal that those lists have been updated, even if this was called and the DB was already up to date with the static QMaps
 */
void TransformationList::loadAllFromStorage(){
    if (Database::connected){
        // Reset shared lists
        this->configMapById.clear();
        this->configVariantMapById.clear();
        QSqlQueryModel queryModel;
        queryModel.setQuery(QStringList({"SELECT * FROM ",TransformationList::TABLENAME}).join(""));
        for (int i=0; i < queryModel.rowCount(); ++i){
            TransformationConfig currConfig = TransformationList::sqlRowToTransformationConfig(queryModel.record(i));
            this->configMapById.insert(currConfig.id,currConfig);
            this->configVariantMapById.insert(currConfig.id,QVariant::fromValue(currConfig));
        }
        // @TODO better checking to see if records have actually changed vs stayed the same
        emit this->transformationsChanged();
    }
    else {
        qDebug() << "Fatal error! DB disconnected!";
    }
}

/**
 * @brief Save a brand new config to the SQL db. Internally calls the insertOrUpdate fn
 * @param newConfig
 * @return id of the inserted row, which should become this configs ID
 */
int TransformationList::saveNewToStorage(TransformationConfig newConfig){
    return TransformationList::insertOrUpdateInStorage(newConfig,true);
}

/**
 * @brief This can be used to either insert a brand new config into the SQL db, or modify an existing one.
 * @param config - Config to insert or update
 * @param isUpdate - Whether or not this should update an existing record in the DB
 * @return ID of the affected SQL row (which should also be the config.id)
 */
int TransformationList::insertOrUpdateInStorage(TransformationConfig config, bool isUpdate){
    int insertedRecordId = -1;
    bool res = false;
    if (Database::connected){
        QSqlQuery insertQuery;
        // Construct SQL query dynamically
        QString query = "REPLACE INTO " + TransformationList::TABLENAME + " (" + (isUpdate ? "id," : "") + "user_defined_name,save_locally,overwrite_local,created_file_suffix,uses_preset,preset_name,uses_named_trans,named_trans,uses_trans_string,trans_string,uses_outgoing_trans,outgoing_trans_string,store_original,delete_cloud_after_download) VALUES(" + (isUpdate ? ":id," : "") + ":user_defined_name,:save_locally,:overwrite_local,:created_file_suffix,:uses_preset,:preset_name,:uses_named_trans,:named_trans,:uses_trans_string,:trans_string,:uses_outgoing_trans,:outgoing_trans_string,:store_original,:delete_cloud_after_download)";
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
        insertQuery.bindValue(":uses_outgoing_trans",config.usesOutgoingTransformationRawString);
        insertQuery.bindValue(":outgoing_trans_string",config.outoingTransformationRawString);
        insertQuery.bindValue(":store_original",config.storeOriginal);
        insertQuery.bindValue(":delete_cloud_after_download",config.deleteCloudCopyAfterDownload);
        res = insertQuery.exec();
        if (res){
            insertedRecordId = insertQuery.lastInsertId().toInt();
            qDebug() << "Config inserted or updated. Id = " << insertedRecordId;
            config.id = insertedRecordId;
            // @TODO is this most efficient way? Could also call loadAllFromStorage for more accuracy, but that uses a SQL call
            // Update the static list instance
            this->addOrUpdateOnSharedLists(config);
            emit TransformationList::getInstance()->transformationsChanged();
        }
    }
    return insertedRecordId;
}

/**
 * @brief Turns a SQL row into a transformation config object
 * @param row - a single SQL row to be parsed
 * @return TransformationConfig result
 */
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
    result.usesOutgoingTransformationRawString = row.value("uses_outgoing_trans").toBool();
    result.outoingTransformationRawString = row.value("outgoing_trans_string").toString();
    result.storeOriginal = row.value("store_original").toBool();
    result.deleteCloudCopyAfterDownload = row.value("delete_cloud_after_download").toBool();
    return result;
}

/**
 * @brief Simply turns the variant QMap into a QVariantList and returns it.
 *
 * Convenient for the QML side, since QVariantList is usually easier to work with natively rather than QMaps
 * @return QVariantList - the converted list
 */
QVariantList TransformationList::getConfigListAsVariantList(){
    return TransformationList::configVariantMapById.values();
}

// @TODO - just fetch off static lits instead of querying db? Or call force db sync first, then fetch off statics?
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

bool TransformationList::deleteConfigByid(int configId){
    bool success = false;

    if (Database::connected){
        QSqlQuery deleteQuery;
        deleteQuery.prepare(QStringList({"DELETE FROM ",TransformationList::TABLENAME," WHERE ",TransformationList::TABLENAME,".id = ",QString::number(configId)}).join(""));
        success = deleteQuery.exec();
        if (success){
            this->removeFromSharedListsByConfigId(configId);
            emit TransformationList::getInstance()->transformationsChanged();
        }
        else {
            qDebug() << "Failed to delete config with id = " << configId;
        }
    }

    return success;
}

/**
 * @brief Adds to or updates a config on the static shared lists.
 * @param config - config to add or update
 */
void TransformationList::addOrUpdateOnSharedLists(TransformationConfig config){
    // REMINDER: For QMap, QMap::insert is equivalent to sql's INSERT OR REPLACE - if key already exists, just updates value instead of inserting.
    TransformationList::getInstance()->configMapById.insert(config.id,config);
    TransformationList::getInstance()->configVariantMapById.insert(config.id,QVariant::fromValue(config));
}

void TransformationList::removeFromSharedListsByConfig(TransformationConfig config){
    TransformationList::removeFromSharedListsByConfigId(config.id);
}

/**
 * @brief This removes a specific config from the static shared QMap config lists
 *
 * NOTE - this does NOT delete the config from the DB. This function should really only be called by the function that actually deletes the DB record first.
 * @param configId - the id (config.id) of the config to remove
 */
void TransformationList::removeFromSharedListsByConfigId(int configId){
    int configMayByIdRemovedCount = TransformationList::getInstance()->configMapById.remove(configId);
    int configVariantMapByIdRemovedCount = TransformationList::getInstance()->configVariantMapById.remove(configId);
    if (configMayByIdRemovedCount == 0 || configVariantMapByIdRemovedCount == 0){
        qDebug() << "Config was not removed from shared lists";
    }
}


