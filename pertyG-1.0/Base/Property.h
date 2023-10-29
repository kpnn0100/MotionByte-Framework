#pragma once
#include "FrameRenderer.h"
namespace pertyG
{
class Property : public IFrameEventListener
{
    double last;
    double lastSetTime;
    double current;
    double target;
    bool isSet = true;
public:
    Property()
    {
        last = 0.0;
        lastSetTime = 0.0;
        current = 0;
        
    }
    void setValue(double value)
    {
        if (target == value)
        {
            //nothing change
            return;
        }
        target = value;
        last = current;
        isSet = false;

    }
    double getValue()
    {
        return current;
    }
    double getTargetValue()
    {
        return target;
    }
    void onFrameInitialized() override
    {
        if (isSet)
        {
            return;
        }
        //do behavior for property change
    }
};
}