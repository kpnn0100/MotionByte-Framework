#pragma once
#include "FrameRenderer.h"
#include <functional>
namespace pertyG
{

class Property : public IFrameEventListener
{
    double last;
    double lastSetTime;
    double current;
    double target;
    bool mIsSet = true;
    std::function<void()> mSetCallback;
public:
    Property()
    {
        last = 0.0;
        lastSetTime = 0.0;
        current = 0;
        
    }
    ~Property()
    {
        
    }
    void setCallback(std::function<void()> function)
    {
        mSetCallback= function;
    }
    void onTargetReached()
    {
        current = target;
        last = target;
        mIsSet = true;
        if (mSetCallback)
        {
            mSetCallback();
        }
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
        mIsSet = false;

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
        if (mIsSet)
        {
            return;
        }
        
        //do behavior for property change
    }
    void onFrameRendered() override
    {
        if (mIsSet)
        {
            return;
        }
        onTargetReached();
    }
    bool isSet()
    {
        return mIsSet;
    }
};
}