#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>

struct settingDbResult {
    bool found;
    QString value;
    settingDbResult(bool found,QString value) :
        found(found),
        value(value)
    {}
};


class GlobalSettings : public QObject
{
    Q_OBJECT
public:
    explicit GlobalSettings(QObject *parent = nullptr);
    static GlobalSettings *getInstance();
    static QString TABLE_NAME;
//    struct settingDbResult {
//        bool found;
//        QString value;
//        settingDbResult(bool found,QString value) :
//            found(found),
//            value(value)
//        {}
//    };
signals:
    void settingsChanged();
public slots:
    bool updateInBulk(QString cloudinaryCloudName, QString cloudinaryApiKey, QString cloudinaryApiSecret);
    void loadFromStorage();
    void saveToStorage();
private:
    static GlobalSettings *m_instance;
    QString m_cloudinaryCloudName;
    QString m_cloudinaryApiKey;
    QString m_cloudinaryApiSecret;
    settingDbResult getSettingStringFromDb(QString settingKey);
    bool saveSettingStringToDb(QString settingKey, QString settingVal);
};

#endif // GLOBALSETTINGS_H
