#include "transformationlist.h"

const QString TransformationList::TABLENAME = "transformationconfigs";

TransformationList::TransformationList(QObject *parent) : QObject(parent)
{

}

TransformationList::transformationConfigList TransformationList::getDefaults(){
    // @TODO
    TransformationList::transformationConfigList defaults;
    // Twitter crop
    TransformationConfig twitterConfig;

    return defaults;
}

void TransformationList::loadAllFromStorage(){
    // @TODO
}

void TransformationList::saveAllToStorage(){
    // @TODO
}

long long TransformationList::saveNewToStorage(TransformationConfig newConfig){
    // @TODO
    return 1;
}

void TransformationList::updateExistingInStorage(){
    // @TODO
}

