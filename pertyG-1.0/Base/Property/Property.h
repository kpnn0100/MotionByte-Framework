#pragma once
#include "../FrameRenderer/FrameRenderer.h"
#include "Interpolator.h"
#include "InterpolatorFactory.h"
#include "../../Functional/Timer.h"
#include <functional>
#include <memory>
#include <atomic>
namespace pertyG
{

class Property : public IFrameEventListener
{
    Timer mInterpolatorTimer;
    std::atomic<double> last;
    double lastSetTime;
    std::atomic<double> current;
    std::atomic<double> target;
    bool mIsSet = true;
    std::shared_ptr<Interpolator> mInterpolator;
    std::function<void()> mSetCallback;
public:
    Property();
    Property(double value);
    Property(const pertyG::Property&)
    {
        
    }
    ~Property();
    operator double() const;
    void initValue(double value);
    void setInterpolator(std::shared_ptr<Interpolator> interpolator);
    void setCallback(std::function<void()> function);
    void onTargetReached();
    void setValue(double value);
    double getValue();
    double getTargetValue();
    void onFrameInitialized() override;
    void onFrameRendered() override;
    bool isSet();
};
}