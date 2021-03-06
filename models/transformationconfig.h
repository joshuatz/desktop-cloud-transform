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
    bool saveLocally = true;
    bool overwriteLocalFile = false;
    QString createdFileSuffix = "";
    bool usesPreset = false;
    QString presetName = "";
    bool usesNamedTransformation = false;
    QString namedTransformation = "";
    bool usesTransformationRawString = false;
    QString transformationRawString = "";
    bool usesOutgoingTransformationRawString = false;
    QString outoingTransformationRawString = "";
    bool storeOriginal = false;
    bool deleteCloudCopyAfterDownload = false;
    // Needed for MOC
    bool operator==(TransformationConfig const &other) const;
    bool operator!=(TransformationConfig const &other) const;
Q_GADGET
    Q_PROPERTY(int id MEMBER id)
    Q_PROPERTY(QString userDefinedName MEMBER userDefinedName)
    Q_PROPERTY(bool saveLocally MEMBER saveLocally)
    Q_PROPERTY(bool overwriteLocalFile MEMBER overwriteLocalFile)
    Q_PROPERTY(QString createdFileSuffix MEMBER createdFileSuffix)
    Q_PROPERTY(bool usesPreset MEMBER usesPreset)
    Q_PROPERTY(QString presetName MEMBER presetName)
    Q_PROPERTY(bool usesNamedTransformation MEMBER usesNamedTransformation)
    Q_PROPERTY(QString namedTransformation MEMBER namedTransformation)
    Q_PROPERTY(bool usesTransformationRawString MEMBER usesTransformationRawString)
    Q_PROPERTY(QString transformationRawString MEMBER transformationRawString)
    Q_PROPERTY(bool usesOutgoingTransformationRawString MEMBER usesOutgoingTransformationRawString)
    Q_PROPERTY(QString outoingTransformationRawString MEMBER outoingTransformationRawString)
    Q_PROPERTY(bool storeOriginal MEMBER storeOriginal)
    Q_PROPERTY(bool deleteCloudCopyAfterDownload MEMBER deleteCloudCopyAfterDownload)
private:

};

Q_DECLARE_METATYPE(TransformationConfig);

#endif // TRANSFORMATIONCONFIG_H
