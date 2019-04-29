#ifndef STATSACTION_H
#define STATSACTION_H

#include <QObject>

class StatsAction : public QObject
{
    Q_OBJECT
public:
    explicit StatsAction(QObject *parent = nullptr);

signals:

public slots:
};

#endif // STATSACTION_H