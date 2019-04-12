#ifndef CLOUDINARY_H
#define CLOUDINARY_H

#include <QObject>
#include <QMap>
#include <QVariant>

class Cloudinary
{
public:
    Cloudinary();
    static QString generateSignature(QMap<QString,QVariant> paramsToSign,QString apiSecret);
private:
};

#endif // CLOUDINARY_H
