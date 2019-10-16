#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QString>

class Logger
{
public:
    static Logger* getInstance();
    Logger(){
        this->_init();
    }
    void clear();
    void logStr(QString msg);
    void refreshLogPrefs();
private:
    static Logger *m_instance;
    void _init();
    bool _logToFile = true;
    QString _logFileDest = "./debug-log.txt";
    bool _userWantsLogging = false;
    QFile* _openLogFile();

};

#endif // LOGGER_H
