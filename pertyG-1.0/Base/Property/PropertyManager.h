#pragma once
#include <vector>
namespace pertyG
{
    class Property;
    class Interpolator;
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
        std::vector<Property*> mPropertyList;
        int setCount = 0;
    public:
        PropertyManager(int propertyCount);
        void setInterpolatorForAll(std::shared_ptr<Interpolator> interpolator);
        void setInterpolator(int id, std::shared_ptr<Interpolator> interpolator);
        void initValue(int id, double value);
        void setValue(int id, double value);
        double getValue(int id);
        double getTargetValue(int id);
        Property& getProperty(int id);
        void setPropertyPointer(int id, Property* pointer);
    };
}