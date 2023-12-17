#pragma once
#include "Property.h"
#include <vector>
namespace pertyG
{
    class IPropertyListener
    {
    public:
        virtual void onSet()
        {

        }
    };
    class PropertyManager
    {
    private:
        std::vector<Property> mPropertyList;
        int setCount = 0;
    public:
        PropertyManager(int propertyCount)
        {
            mPropertyList.resize(propertyCount);
            for (auto &property: mPropertyList)
            {
                property.setCallback([this]{
                    onSetCallback();
                    });
            }
            setCount = propertyCount;
        }
        void initValue(int id, double value)
        {
            mPropertyList[id].initValue(value);
        }
        void setValue(int id, double value)
        {
            if (mPropertyList[id].getTargetValue() == value)
            {
                return;
            }
            if (mPropertyList[id].isSet())
            {
                setCount-=1;
            }
            mPropertyList[id].setValue(value);
        }
        void onSetCallback()
        {
            setCount+=1;
            std::cout << "set count" << setCount;
        }
        double getValue(int id)
        {
            return mPropertyList[id].getValue();
        }
        double getTargetValue(int id)
        {
            return mPropertyList[id].getTargetValue();
        }
        bool isSet()
        {
            return setCount == mPropertyList.size();
        }
        Property& getProperty(int id)
        {
            return mPropertyList[id];
        }
    };
}