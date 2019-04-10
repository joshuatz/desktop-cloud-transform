#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

class Database
{
public:
    #ifdef QT_DEBUG
    QString DB_PATH = "C:/Users/Joshua/Downloads/Temp/DCT/dct.db";
    #else
    QString DB_PATH = "dct.db";
    #endif
    static QSqlDatabase db;
    static bool connected;
    static Database* getInstance();
    Database(){
        this->connected = false;
        this->connect();
    }
    ~Database(){
        if (connected){
            Database::db.close();
        }

    }
private:
    bool connect(){
        bool connectionEstablished = false;
        QSqlDatabase dbSetup = QSqlDatabase::addDatabase("QSQLITE");
        dbSetup.setDatabaseName(this->DB_PATH);
        connectionEstablished = dbSetup.open();
        Database::db = dbSetup;
        this->connected = connectionEstablished;
        return connectionEstablished;
    }
    static Database *m_instance;
};

#endif // DATABASE_H
