#include "database.h"

QSqlDatabase Database::db;
bool Database::connected = false;

Database *Database::m_instance = nullptr;

Database *Database::getInstance(){
    if (m_instance == nullptr){
        m_instance = new Database();
    }
    return m_instance;
}
