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
signals:
    void transformationsChanged();
public slots:
    void loadAllFromStorage();
    void saveAllToStorage();
    int saveNewToStorage(TransformationConfig newConfig);
    void updateExistingInStorage();
    TransformationList::ConfigList getDefaults();
private:
    static TransformationList *m_instance;
    static const QString TABLENAME;
    static TransformationConfig sqlRowToTransformationConfig(QSqlRecord row);
};

Q_DECLARE_METATYPE(TransformationList::ConfigMapByIdType);
Q_DECLARE_METATYPE(TransformationList::configVariantMapByIdType);

#endif // TRANSFORMATIONLIST_H
