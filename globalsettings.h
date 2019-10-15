#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>
#include <QVariant>

struct settingDbResult {
    bool found;
    QVariant value;
    settingDbResult(bool found,QVariant value) :
        found(found),
        value(value)
    {}
};

class GlobalSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool cloudinaryConfigured MEMBER m_cloudinaryProperlyConfigured NOTIFY settingsChanged)
    Q_PROPERTY(QString cloudinaryCloudName MEMBER m_cloudinaryCloudName NOTIFY settingsChanged)
    Q_PROPERTY(bool optedOutTracking MEMBER m_optedOutTracking NOTIFY settingsChanged)
    Q_PROPERTY(bool userDebugLogOn MEMBER m_userDebugLogOn NOTIFY settingsChanged)
public:
    explicit GlobalSettings(QObject *parent = nullptr);
    static GlobalSettings *getInstance();
    static QString TABLE_NAME;
    bool validateCloudinarySettings();
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
    void emitSettingsChanged(){
        emit settingsChanged();
    }
private:
    static GlobalSettings *m_instance;
    bool m_cloudinaryProperlyConfigured = false;
    QString m_cloudinaryCloudName;
    QString m_cloudinaryApiKey;
    QString m_cloudinaryApiSecret;
    bool m_hasInternet = false;
    settingDbResult getSettingFromDb(QString settingKey);
    bool saveSettingToDb(QString settingKey, QVariant settingVal);
    bool m_userDebugLogOn = false;
    bool m_optedOutTracking = false;
};

#endif // GLOBALSETTINGS_H
