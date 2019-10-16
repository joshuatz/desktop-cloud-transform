#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>

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
    bool _openLogFile(QFile *filePtr);
    QFile* _openLogFile();
    FILE* _openLogFileOs();

};

#endif // LOGGER_H
