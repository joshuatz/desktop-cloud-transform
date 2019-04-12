#ifndef HELPERS_H
#define HELPERS_H

#include <QObject>

class Helpers : public QObject
{
    Q_OBJECT
public:
    explicit Helpers(QObject *parent = nullptr);
    static void checkInternetConnection(bool *res);
signals:
public slots:
    static bool checkValidImageFilePath(QString imagePath);
private:
};

#endif // HELPERS_H
