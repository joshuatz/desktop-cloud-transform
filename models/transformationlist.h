#ifndef TRANSFORMATIONLIST_H
#define TRANSFORMATIONLIST_H

#include <QObject>
#include <QMap>
#include "transformationconfig.h"

class TransformationList : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(TransformationList::transformationConfigList list MEMBER list NOTIFY transformationsChanged)
public:
    explicit TransformationList(QObject *parent = nullptr);
    typedef QList<TransformationConfig> transformationConfigList;
    QMap<long long,TransformationConfig> transformationConfigListById;
    TransformationList::transformationConfigList list;
signals:
    void transformationsChanged();
public slots:
    void loadAllFromStorage();
    void saveAllToStorage();
    long long saveNewToStorage(TransformationConfig newConfig);
    void updateExistingInStorage();
    TransformationList::transformationConfigList getDefaults();
private:
    static const QString TABLENAME;
};

#endif // TRANSFORMATIONLIST_H
