#include "logger.h"
#include "globalsettings.h"
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include "helpers.h"

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

bool Logger::_openLogFile(QFile *filePtr) {
    try {
        filePtr = new QFile(_logFileDest);
        //QFile file(_logFileDest);
        //filePtr = &file;
        filePtr->open(QIODevice::Append | QIODevice::Text);
        QTextStream out(filePtr);
        out << "\n\nOPENED!";
        return true;
    } catch (...) {
        filePtr = nullptr;
        return false;
    }
}

QFile *Logger::_openLogFile() {
    QFile *filePtr = nullptr;
    try {
        filePtr = new QFile(_logFileDest);
        filePtr->open(QIODevice::Append | QIODevice::Text);
        return filePtr;
    } catch (...) {
        return filePtr;
    }
}

FILE *Logger::_openLogFileOs() {
    FILE* filePtr = nullptr;
    try {
        QFile qfileObj(_logFileDest);
        qfileObj.open(QIODevice::Append | QIODevice::Text);
        int fileHndl = qfileObj.handle();
        filePtr = _fdopen(fileHndl, "ab");
        return filePtr;
    } catch (...) {
        return filePtr;
    }
}

void Logger::clear() {
    if (_logToFile) {
        QFile *file = nullptr;
        if(this->_openLogFile(file)) {
            file->resize(0);
            file->close();
        }
    }
}

void Logger::logStr(QString msg) {
    if (_logToFile) {

        QFile* file;
        /*
        if(this->_openLogFile(&file)) {
            //file.open(QIODevice::Append | QIODevice::Text);
            QTextStream out(&file);
            //file.write("FOOBAR");
            out << msg;
            file.close();
        }
        */

        /*
        FILE *file = this->_openLogFileOs();
        QTextStream out(file);
        out << msg;
        fclose(file);
        */

        file = this->_openLogFile();
        QTextStream out(file);
        out << msg;
        file->close();
    }
    if (Helpers::getIsDebug()) {
        qDebug() << msg;
    }
}
