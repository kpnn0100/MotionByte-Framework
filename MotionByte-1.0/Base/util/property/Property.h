#pragma once
#include <functional>
#include <memory>
#include <atomic>

#include "animator/timer/Timer.h"
#include "util/functional/GraphicMath.h"
#include "InterpolatorModule.h"
namespace MotionByte
{
class InterpolatorModule;
class Property
{
private:
    Timer mInterpolatorTimer;
    std::function<double()> mBindFunction;
    std::atomic<double> last;
    double lastSetTime;
    std::atomic<double> current;
    std::atomic<double> target;
    std::atomic<double> lastVelocity;
    bool mIsSet = true;
    InterpolatorModule mInterpolator;
    std::function<void()> mSetCallback;
    std::string mPropertyName;
    void update();
    friend class InterpolatorModule;
    friend class InterpolatorBase;
    double mMin;
    double mMax;
public:
    std::vector<double>& getInterpolatorState();
    InterpolatorModule getInterpolator();
    InterpolatorType getInterpolatorType();
    Property();
    Property(double value);
    Property(const Property& other);
    ~Property();
    operator double();
    bool operator>(Property& other);
    bool operator>=(Property& other);
    bool operator<(Property& other);
    bool operator<=(Property& other);

    Property& operator=(double value);
    Property& operator=(const Property & other);
    void setPropertyName(std::string propertyName);
    std::string getName();
    void initValue(double value);
    void setInterpolator(InterpolatorModule InterpolatorModule);
    void setCallback(std::function<void()> function);
    void onTargetReached();
    void setValue(double value);
    Property shift(double value);
    void bind(std::function<double()> bindFunction);
    void removeBind();
    double getLastVelocity();
    double getLastValue();
    double getValue();
    double getTargetValue();
    double getElapsedTime();
    void setMin(double min);
    void setMax(double max);
    void unsetMin();
    void unsetMax();
    bool isSet();
};
}