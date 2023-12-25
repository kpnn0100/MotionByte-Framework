#pragma once
#include <functional>
#include <memory>
#include <atomic>

#include "../FrameRenderer/FrameRenderer.h"
#include "Interpolator.h"
#include "InterpolatorFactory.h"
#include "../../Functional/Timer.h"

namespace pertyG
{

class Property
{
    Timer mInterpolatorTimer;
    std::atomic<double> last;
    double lastSetTime;
    std::atomic<double> current;
    std::atomic<double> target;
    bool mIsSet = true;
    std::shared_ptr<Interpolator> mInterpolator;
    std::function<void()> mSetCallback;
    void update();
public:
    Property();
    Property(double value);
    Property(const Property& other);
    ~Property();
    operator double();
    Property& operator=(double value);
    Property& operator=(const Property & other);
    
    void initValue(double value);
    void setInterpolator(std::shared_ptr<Interpolator> interpolator);
    void setCallback(std::function<void()> function);
    void onTargetReached();
    void setValue(double value);
    Property shift(double value);
    double getValue();
    double getTargetValue();
    bool isSet();
};
}