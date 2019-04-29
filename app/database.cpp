#include "database.h"
#include "globalsettings.h"
#include "../lib/mousaviexecutesqlfile.h"
#include "models/transformationlist.h"
#include <QDebug>

bool Database::connected = false;
QString Database::m_createTablesSqlFilePath = ":/db/create_tables.sql";

Database *Database::m_instance = nullptr;

Database *Database::getInstance(){
    if (m_instance == nullptr){
        m_instance = new Database();
    }
    return m_instance;
}

/**
 * @brief Database::getDb - returns primary db instead of storing as static member - see QT warning about having DB instance as member
 * @return QSqlDatabase - default db attached
 */
QSqlDatabase Database::getDb(){
    Database::getInstance()->connect();
    // Empty connection name to return default (primary) db
    return QSqlDatabase::database();
}

bool Database::connect(){
    if (this->connected==false){
        // check if the DB file at least exists
        bool connectionEstablished = false;
        QSqlDatabase dbSetup = QSqlDatabase::addDatabase("QSQLITE");
        dbSetup.setDatabaseName(this->DB_PATH);
        // NOTE - calling QSqlDatabase.open on a Sqlite path that does not exist yet, will (or should) actually create a new DB file at that path
        connectionEstablished = dbSetup.open();
        if (!connectionEstablished){
            // Hmmm. calling db open should have created file, but maybe it didn't.
            if (!QFile::exists(this->DB_PATH)){
                // This should create file if it does not exist
                QFile dbFile(this->DB_PATH);
                dbFile.open(QIODevice::WriteOnly);
                dbFile.close();
                // Try again
                connectionEstablished = dbSetup.open();
            }
        }
        this->connected = connectionEstablished;
        if (connectionEstablished){
            // Set options - must be done after open
            dbSetup.exec("PRAGMA foreign_keys = ON;");
            if (dbSetup.tables(QSql::Tables).contains(GlobalSettings::TABLE_NAME) == false){
                // This must be a blank new DB, so we need to set up the empty tables for the app
                this->createTables();
            }
        }
        return connectionEstablished;
    }
    return true;
}

void Database::createTables(){
    // Code to create DB tables is stored as one single SQL file with a bunch of "CREATE TABLE" statements
    if (QFile::exists(this->m_createTablesSqlFilePath)){
        QFile sqlFile(this->m_createTablesSqlFilePath);
        QSqlDatabase db = this->getDb();
        MousaviExecuteSqlFile::executeQueryFile(sqlFile,db);
        this->seedTables();
    }
    else {
        qDebug() << "Missing create_tables sql file";
    }
}

void Database::seedTables(){
    if (Database::connected){
        // Load in default configs
        TransformationList::ConfigList defaults = TransformationList::getDefaults();
        TransformationList::ConfigList::iterator i;
        for (i = defaults.begin(); i != defaults.end(); ++i){
            TransformationList::getInstance()->saveNewToStorage(*i);
        }
    }
}
