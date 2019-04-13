#ifndef STATS_H
#define STATS_H

#include <QObject>

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
private:
    static Stats *m_instance;
    static const QString TABLENAME;
};

#endif // STATS_H
