#ifndef TRANSFORMATIONCONFIG_H
#define TRANSFORMATIONCONFIG_H

#include <QObject>

class TransformationConfig : public QObject
{
    Q_OBJECT
public:
    explicit TransformationConfig(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TRANSFORMATIONCONFIG_H