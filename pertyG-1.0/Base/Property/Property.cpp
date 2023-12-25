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
    void Property::update()
    {
        if (mIsSet)
        {
            return;
        }
        current = mInterpolator->getValueAtTime(last, target, mInterpolatorTimer.getDuration());
        if (mInterpolator->isSet(mInterpolatorTimer.getDuration()))
        {
            onTargetReached();
        }
    }
    Property::operator double()
    {
        return getValue();
    }
    Property& Property::operator=(double value)
    {
        setValue(value);
        update();
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
            update();
        }
        return *this;
    }
    void Property::initValue(double value)
    {
        last = value;
        current = value;
        target = value;
        mIsSet = true;
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
        initValue((double)target);
        if (mSetCallback)
        {
            mSetCallback();
        }
    }
    void Property::setValue(double value)
    {
        update();
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
    Property Property::shift(double value)
    {
        update();
        Property shiftProperty = *this;
        shiftProperty.last.store(shiftProperty.last.load() + value);
        shiftProperty.target.store(shiftProperty.target.load() + value);
        return shiftProperty;
    }
    double Property::getValue()
    {
        update();
        return current;
    }
    double Property::getTargetValue()
    {
        update();
        return target;
    }
    bool Property::isSet()
    {
        update();
        return mIsSet;
    }
}