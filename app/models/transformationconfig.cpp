#include "transformationconfig.h"

TransformationConfig::TransformationConfig()
{

}

/*
 * Needed for MOC
 */
bool TransformationConfig::operator==(const TransformationConfig &other) const{
    // Compare
    return true;
}
bool TransformationConfig::operator!=(const TransformationConfig &other) const{
    return !(*this==other);
}
