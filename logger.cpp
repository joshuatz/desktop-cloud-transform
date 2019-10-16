#include "globalsettings.h"
#include "helpers.h"
#include "logger.h"
#include <QDebug>
#include <QFileInfo>
#include <QTextStream>

Logger *Logger::m_instance = nullptr;

Logger *Logger::getInstance(){
    if (m_instance == nullptr){
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::_init() {
    this->refreshLogPrefs();
    if (_userWantsLogging && _logToFile) {
        // Make sure log file exists
        QFile file(_logFileDest);
        if (!file.exists()) {
            qDebug() << "log file does not exist -> creating now";
            // Create
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            file.write("");
            file.close();
        }
        // Double check
        _logToFile = QFileInfo::exists(_logFileDest);
    }
}

void Logger::refreshLogPrefs() {
    this->_userWantsLogging =  GlobalSettings::getInstance()->getUserDebugLogOn();
    // For now, no separate setting for file vs other
    this->_logToFile = this->_userWantsLogging;
}

QFile *Logger::_openLogFile() {
    QFile *filePtr = nullptr;
    try {
        filePtr = new QFile(_logFileDest);
        filePtr->open(QIODevice::Append | QIODevice::Text);
    } catch (...) {
        qDebug() << "Could not open debug-log file";
    }
    return filePtr;
}

void Logger::clear() {
    if (_logToFile) {
        QFile *file = this->_openLogFile();
        file->resize(0);
        file->close();
    }
}

void Logger::logStr(QString msg) {
    if (_logToFile) {
        QFile *file = this->_openLogFile();
        QTextStream out(file);
        out << "\n" << msg;
        file->close();
    }
    if (Helpers::getIsDebug()) {
        qDebug() << msg;
    }
}
