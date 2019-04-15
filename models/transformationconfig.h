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
    int id = -1;
    QString userDefinedName = "";
    bool overwriteLocalFile = false;
    QString createdFileSuffix = "";
    bool usesPreset = false;
    QString presetName = "";
    bool usesTransformationRawString = false;
    QString transformationRawString = "";
    bool storeOriginal = false;
    bool deleteCloudCopyAfterDownload = false;
    // Needed for MOC
    bool operator==(TransformationConfig const &other) const;
    bool operator!=(TransformationConfig const &other) const;
Q_GADGET
    Q_PROPERTY(int id MEMBER id)
    Q_PROPERTY(QString userDefinedName MEMBER userDefinedName)
private:

};

Q_DECLARE_METATYPE(TransformationConfig);

#endif // TRANSFORMATIONCONFIG_H
