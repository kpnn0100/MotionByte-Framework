#include "Property.h"
namespace pertyG
{
    Property::Property() : Property (0.0)
    {

    }
    Property::Property(double value)
    {
        last =value;
        lastSetTime = value;
        current =value;
        target = value;
        mInterpolator = InterpolatorFactory::createStep();
    }
    Property::Property(const Property& other)
    {
        *this = other;
    }
    Property::~Property()
    {

    }
    Property::operator double()
    {
        return getValue();
    }
    Property& Property::operator=(double value)
    {
        setValue(value);
        return *this;
    }
    Property& Property::operator=(const Property& other)
    {
        if (this != &other) {
            // Copy non-atomic members
            mInterpolatorTimer = other.mInterpolatorTimer;
            lastSetTime = other.lastSetTime;
            mIsSet = other.mIsSet;
            mInterpolator = other.mInterpolator;
            mSetCallback = other.mSetCallback;

            // Note: If your class contains dynamic memory or other resources,
            // you might need to perform deep copying or resource management here.

            // For atomic types, you can use the atomic store member function
            last.store(other.last.load());
            current.store(other.current.load());
            target.store(other.target.load());

            // Assuming Interpolator is copyable
            mInterpolator = other.mInterpolator;

            // Assuming std::function is copyable
            mSetCallback = other.mSetCallback;
        }
        return *this;
    }
    void Property::initValue(double value)
    {
        last = value;
        current = value;
        target = value;
    }
    void Property::setInterpolator(std::shared_ptr<Interpolator> interpolator)
    {
        mInterpolator = interpolator;
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
    Property Property::shift(double value) const
    {
        Property shiftProperty = *this;
        shiftProperty.last.store(shiftProperty.last.load() + value);
        shiftProperty.target.store(shiftProperty.target.load() + value);
        return shiftProperty;
    }
    double Property::getValue()
    {
        current = mInterpolator->getValueAtTime(last, target, mInterpolatorTimer.getDuration());
        if (mInterpolator->isSet(mInterpolatorTimer.getDuration()))
        {
            onTargetReached();
        }
        return current;
    }
    double Property::getTargetValue()
    {
        return target;
    }
    bool Property::isSet()
    {
        return mIsSet;
    }
}