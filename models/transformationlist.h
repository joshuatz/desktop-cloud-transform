#ifndef TRANSFORMATIONLIST_H
#define TRANSFORMATIONLIST_H

#include <QObject>

class TransformationList : public QObject
{
    Q_OBJECT
public:
    explicit TransformationList(QObject *parent = nullptr);

signals:
    void transformationsChanged();
public slots:
private:
};

#endif // TRANSFORMATIONLIST_H
