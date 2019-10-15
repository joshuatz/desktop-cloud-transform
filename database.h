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
    QString DB_PATH = "dct-debug.db";
    #else
    QString DB_PATH = "dct.db";
    #endif
    static QSqlDatabase getDb();
    static bool connected;
    static Database* getInstance();
    Database(){
        this->connect();
    }
    ~Database(){
        if (connected){
            this->getDb().close();
        }

    }
private:
    bool connect();
    void createTables();
    void seedTables();
    static Database *m_instance;
    static QString m_createTablesSqlFilePath;
};

#endif // DATABASE_H
