#include "stats.h"

const QString Stats::TABLENAME = "stats";
Stats *Stats::m_instance = nullptr;

Stats *Stats::getInstance(){
    if(m_instance==nullptr){
        m_instance = new Stats();
    }
    return m_instance;
}



Stats::Stats(QObject *parent) : QObject(parent)
{

}

void Stats::forceRefreshFromDb(){
    // @TODO
}
