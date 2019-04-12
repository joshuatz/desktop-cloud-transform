#ifndef TRANSFORMATIONCONFIG_H
#define TRANSFORMATIONCONFIG_H

#include <QObject>

class TransformationConfig
{
public:
    TransformationConfig();
    enum ConfigDestination {
        CLOUDINARY
    };
    ConfigDestination destination = CLOUDINARY;
    long long id;

private:

};

#endif // TRANSFORMATIONCONFIG_H
