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
    bool usesPreset = false;
    QString presetName = "";
    bool usesTransformationRawString = false;
    QString transformationRawString = "";
    bool storeOriginal = false;
    bool deleteCloudCopyAfterDownload = false;
    Q_GADGET
        Q_PROPERTY(long long id MEMBER id)
private:

};

Q_DECLARE_METATYPE(TransformationConfig);

#endif // TRANSFORMATIONCONFIG_H
