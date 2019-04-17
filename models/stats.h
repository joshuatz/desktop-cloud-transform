#ifndef STATS_H
#define STATS_H

#include <QObject>
#include "transformationconfig.h"

class Stats : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int totalUploads MEMBER totalUploads NOTIFY statsUpdated)
public:
    explicit Stats(QObject *parent = nullptr);
    static Stats *getInstance();
    int totalUploads;
signals:
    void statsUpdated();
public slots:
    void forceRefreshFromDb();
    static int logStat(QString category, QString action, bool usedCloudinary);
    static int logStatWithConfig(QString category,QString action,bool usedCloudinary,bool usedConfig,int configId);
private:
    static Stats *m_instance;
    static const QString TABLENAME;
};

#endif // STATS_H
