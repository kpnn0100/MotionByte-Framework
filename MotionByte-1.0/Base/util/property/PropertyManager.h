#pragma once
#include <vector>
namespace MotionByte
{
    class Property;
    class InterpolatorModule;
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
        void setInterpolatorForAll(InterpolatorModule InterpolatorModule);
        void setInterpolator(int id, InterpolatorModule InterpolatorModule);
        void initValue(int id, double value);
        void setValue(int id, double value);
        double getValue(int id);
        double getTargetValue(int id);
        Property& getProperty(int id);
        void setPropertyPointer(int id, Property* pointer);
    };
}