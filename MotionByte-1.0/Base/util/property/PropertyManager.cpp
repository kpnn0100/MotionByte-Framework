#include "PropertyHeader.h"
namespace MotionByte
{
    PropertyManager::PropertyManager(int propertyCount) : mPropertyList(propertyCount)
    {
    }
    void PropertyManager::setInterpolatorForAll(InterpolatorModule InterpolatorModule)
    {
        for (auto& property : mPropertyList)
        {
            property->setInterpolator(InterpolatorModule);
        }
    }
    void PropertyManager::setInterpolator(int id, InterpolatorModule InterpolatorModule)
    {
        mPropertyList[id]->setInterpolator(InterpolatorModule);
    }
    void PropertyManager::initValue(int id, double value)
    {
        mPropertyList[id]->initValue(value);
    }
    void PropertyManager::setValue(int id, double value)
    {
        if (mPropertyList[id]->getTargetValue() == value)
        {
            return;
        }
        mPropertyList[id]->setValue(value);
    }
    double PropertyManager::getValue(int id)
    {
        return mPropertyList[id]->getValue();
    }
    double PropertyManager::getTargetValue(int id)
    {
        return mPropertyList[id]->getTargetValue();
    }
    Property& PropertyManager::getProperty(int id)
    {
        return *mPropertyList[id];
    }
    void PropertyManager::setPropertyPointer(int id, Property* pointer)
    {
        mPropertyList[id] = pointer;
    }
}