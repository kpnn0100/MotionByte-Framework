#include "Property.h"
namespace pertyG
{
    Property::Property()
    {
        last = 0.0;
        lastSetTime = 0.0;
        current = 0;
        mInterpolator = InterpolatorFactory::createStep();
    }
    Property::~Property()
    {

    }
    void Property::setCallback(std::function<void()> function)
    {
        mSetCallback = function;
    }
    void Property::onTargetReached()
    {
        current = (double)target;
        last = (double)target;
        mIsSet = true;
        if (mSetCallback)
        {
            mSetCallback();
        }
    }
    void Property::setValue(double value)
    {
        if (target == value)
        {
            //nothing change
            return;
        }
        target = value;
        last = (double)current;
        mIsSet = false;
        mInterpolatorTimer.restart();
    }
    double Property::getValue()
    {
        return current;
    }
    double Property::getTargetValue()
    {
        return target;
    }
    void Property::onFrameInitialized()
    {
        if (mIsSet)
        {
            return;
        }

        //do behavior for property change
        current = mInterpolator->getValueAtTime(last,target,mInterpolatorTimer.getDuration());
        if (mInterpolator->isSet(mInterpolatorTimer.getDuration()))
        {
            onTargetReached();
        }
    }
    void Property::onFrameRendered()
    {

    }
    bool Property::isSet()
    {
        return mIsSet;
    }
}