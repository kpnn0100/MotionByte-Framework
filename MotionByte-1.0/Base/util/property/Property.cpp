#include "PropertyHeader.h"
#include <limits>
namespace MotionByte
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
        unsetMin();
        unsetMax();
    }
    Property::Property(const Property& other):Property()
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
        current = mInterpolator.getValueAtTime(*this);
        if (mInterpolator.isSet(*this))
        {
            onTargetReached();
        }

    }
    std::vector<double>& Property::getInterpolatorState()
    {
        return mInterpolator.getInterpolatorState();
    }
    InterpolatorModule Property::getInterpolator()
    {
        return mInterpolator;
    }
    InterpolatorType Property::getInterpolatorType()
    {
        return mInterpolator.getInterpolatorType();
    }
    bool Property::operator>(Property& other) {
        return getValue() > other.getValue();
    }

    bool Property::operator<(Property& other) {
        return getValue() < other.getValue();
    }

    bool Property::operator>=(Property& other) {
        return getValue() >= other.getValue();
    }

    bool Property::operator<=(Property& other) {
        return getValue() <= other.getValue();
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
            //I mean get value should be const right?
            setValue(((Property*)(&other))->getValue());
            update();
        }
        return *this;
    }
    void Property::setPropertyName(std::string propertyName)
    {
        mPropertyName = propertyName;
    }
    void Property::initValue(double value)
    {
        last = value;
        current = value;
        target = value;
        mIsSet = true;
    }
    void Property::setInterpolator(InterpolatorModule InterpolatorModule)
    {
        mInterpolator = InterpolatorModule;
        mInterpolator.updateStateFor(*this);
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
        value = clamp(value, mMin, mMax);
        lastVelocity = mInterpolator.getVelocityAtTime(*this);
        target = value;
        last = (double)current;
        mIsSet = false;
        mInterpolator.updateStateFor(*this);
        mInterpolatorTimer.restart();
    }
    Property Property::shift(double value)
    {
        update();
        Property shiftProperty = *this;
        shiftProperty.last.store(shiftProperty.last.load() + value);
        shiftProperty.current.store(shiftProperty.current.load() + value);
        shiftProperty.target.store(shiftProperty.target.load() + value);
        shiftProperty.mInterpolator.updateStateFor(shiftProperty);
        return shiftProperty;
    }
    void Property::bind(std::function<double()> bindFunction)
    {
        mBindFunction = bindFunction;
    }
    void Property::removeBind()
    {
        mBindFunction = std::function<int()>();
    }
    double Property::getLastVelocity()
    {
        return lastVelocity.load();
    }
    double Property::getLastValue()
    {
        return last;
    }
    double Property::getValue()
    {
        if (mBindFunction)
        {
            return clamp(mBindFunction(),mMin,mMax);
        }
        update();
        return clamp(current, mMin, mMax);
    }
    double Property::getTargetValue()
    {
        return target;
    }
    double Property::getElapsedTime()
    {
        return mInterpolatorTimer.getDuration();
    }
    void Property::setMin(double min)
    {
        mMin = min;
    }
    void Property::setMax(double max)
    {
        mMax = max;
    }
    void Property::unsetMin()
    {
        mMin = std::numeric_limits<double>::lowest();
    }
    void Property::unsetMax()
    {
        mMax = std::numeric_limits<double>::max();
    }
    bool Property::isSet()
    {
        update();
        return mIsSet;
    }
}