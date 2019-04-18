#ifndef STATS_H
#define STATS_H

#include "ganalytics.h"
#include "transformationconfig.h"
#include <QObject>
#include <QVariant>

class Stats : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int cloudinaryTotalUploads MEMBER m_cloudinaryUploadCount NOTIFY statsUpdated)
    Q_PROPERTY(int cloudinaryTotalDownloads MEMBER m_cloudinaryDownloadCount NOTIFY statsUpdated)
public:
    explicit Stats(QObject *parent = nullptr);
    static Stats *getInstance();
    static GAnalytics *getGaTracker();
signals:
    void statsUpdated();
public slots:
    void forceRefreshFromDb();
    static void fireGaEvent(QString category, QString action, QString label, QVariant value);
    /**
     * @brief logStat - logStat and logStatWithConfig both log internally - by saving to DB.
     */
    static int logStat(QString category, QString action, bool usedCloudinary);
    static int logStatWithConfig(QString category,QString action,bool usedCloudinary,bool usedConfig,int configId);
private:
    static Stats *m_instance;
    static const QString TABLENAME;
    static GAnalytics *m_gaTrackerInstance;
    static int m_cloudinaryUploadCount;
    static int m_cloudinaryDownloadCount;
    static int getCountOfStatOverDuration(QDateTime startDate,QDateTime endDate, QString category, QString action, bool usedCloudinary, int configId);
    static int getCountOfStatAllTime(QString category, QString action, bool usedCloudinary, int configId);
};

#endif // STATS_H
