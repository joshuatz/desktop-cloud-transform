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
    Q_PROPERTY(QString cloudinaryCloudName MEMBER m_cloudinaryCloudName NOTIFY settingsChanged)
public:
    explicit GlobalSettings(QObject *parent = nullptr);
    static GlobalSettings *getInstance();
    static QString TABLE_NAME;
signals:
    void settingsChanged();
    void internetConnectionChanged();
public slots:
    bool updateInBulk(QString cloudinaryCloudName, QString cloudinaryApiKey, QString cloudinaryApiSecret);
    void loadFromStorage();
    void saveToStorage();
    QString getCloudinaryCloudName(){
        return m_cloudinaryCloudName;
    }
    QString getCloudinaryApiKey(){
        return m_cloudinaryApiKey;
    }
    QString getCloudinaryApiSecret(){
        return m_cloudinaryApiSecret;
    }
private:
    static GlobalSettings *m_instance;
    QString m_cloudinaryCloudName;
    QString m_cloudinaryApiKey;
    QString m_cloudinaryApiSecret;
    bool m_hasInternet = false;
    settingDbResult getSettingStringFromDb(QString settingKey);
    bool saveSettingStringToDb(QString settingKey, QString settingVal);
};

#endif // GLOBALSETTINGS_H
