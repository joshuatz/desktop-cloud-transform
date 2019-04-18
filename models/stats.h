#ifndef STATS_H
#define STATS_H

#include "ganalytics.h"
#include "transformationconfig.h"
#include <QObject>
#include <QVariant>

class Stats : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int totalUploads MEMBER totalUploads NOTIFY statsUpdated)
public:
    explicit Stats(QObject *parent = nullptr);
    static Stats *getInstance();
    int totalUploads;
    static GAnalytics *getGaTracker();
signals:
    void statsUpdated();
public slots:
    void forceRefreshFromDb();
    static void fireGaEvent(QString category, QString action, QString label, QVariant value);
    static int logStat(QString category, QString action, bool usedCloudinary);
    static int logStatWithConfig(QString category,QString action,bool usedCloudinary,bool usedConfig,int configId);
private:
    static Stats *m_instance;
    static const QString TABLENAME;
    static GAnalytics *m_gaTrackerInstance;
};

#endif // STATS_H
