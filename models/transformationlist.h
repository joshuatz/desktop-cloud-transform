#ifndef TRANSFORMATIONLIST_H
#define TRANSFORMATIONLIST_H

#include <QObject>
#include "transformationconfig.h"

class TransformationList : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(TransformationList::transformationConfigList list MEMBER list NOTIFY transformationsChanged)
public:
    explicit TransformationList(QObject *parent = nullptr);
    typedef QList<TransformationConfig> transformationConfigList;
    TransformationList::transformationConfigList list;
signals:
    void transformationsChanged();
public slots:
    void loadFromStorage();
    void saveToStorage();
    TransformationList::transformationConfigList getDefaults();
private:
};

#endif // TRANSFORMATIONLIST_H
