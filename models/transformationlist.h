#ifndef TRANSFORMATIONLIST_H
#define TRANSFORMATIONLIST_H

#include <QObject>
#include <QMap>
#include <QSqlRecord>
#include <QVariantList>
#include "transformationconfig.h"

class TransformationList : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(TransformationList::ConfigListById transformationConfigListById MEMBER transformationConfigListById NOTIFY transformationsChanged)
    Q_PROPERTY(QVariantList configList READ getConfigListAsVariantList NOTIFY transformationsChanged)
public:
    explicit TransformationList(QObject *parent = nullptr);
    static TransformationList *getInstance();
    typedef QList<TransformationConfig> ConfigList;
    typedef QMap<int,TransformationConfig> ConfigMapByIdType;
    typedef QMap<int,QVariant> configVariantMapByIdType;
    static QMap<int,TransformationConfig> configMapById;
    static QMap<int,QVariant> configVariantMapById;
    static QVariantList getConfigListAsVariantList();
    static TransformationConfig getConfigById(int id);
    static QMap<QString,QVariant> configToParams(TransformationConfig config);
signals:
    void transformationsChanged();
public slots:
    void loadAllFromStorage();
    void saveAllToStorage();
    int saveNewToStorage(TransformationConfig newConfig);
    int insertOrUpdateInStorage(TransformationConfig config,bool isUpdate);
    void updateExistingInStorage();
    TransformationList::ConfigList getDefaults();
    TransformationConfig getBlankTransformationConfig(){
        TransformationConfig config;
        return config;
    }
    bool deleteConfigByid(int configId);
private:
    static TransformationList *m_instance;
    static const QString TABLENAME;
    static TransformationConfig sqlRowToTransformationConfig(QSqlRecord row);
};

Q_DECLARE_METATYPE(TransformationList::ConfigMapByIdType);
Q_DECLARE_METATYPE(TransformationList::configVariantMapByIdType);

#endif // TRANSFORMATIONLIST_H
