#pragma once
#include <functional>
#include <memory>
#include <atomic>

#include "../FrameRenderer/FrameRenderer.h"
#include "../../Functional/Timer.h"
#include "../../Functional/GraphicMath.h"
namespace MotionByte
{
class Interpolator;
class Property
{
private:
    std::vector<double> mInterpolatorState;
    Timer mInterpolatorTimer;
    std::function<double()> mBindFunction;
    std::atomic<double> last;
    double lastSetTime;
    std::atomic<double> current;
    std::atomic<double> target;
    std::atomic<double> lastVelocity;
    bool mIsSet = true;
    std::shared_ptr<Interpolator> mInterpolator;
    std::function<void()> mSetCallback;
    std::string mPropertyName;
    void update();
    friend class Interpolator;
    double mMin;
    double mMax;
public:
    std::vector<double>& getInterpolatorState();
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
    void initValue(double value);
    void setInterpolator(std::shared_ptr<Interpolator> interpolator);
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