#include "transformationlist.h"

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

void TransformationList::loadFromStorage(){
    // @TODO
}

void TransformationList::saveToStorage(){
    // @TODO
}

