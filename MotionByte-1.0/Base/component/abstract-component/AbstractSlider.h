#pragma once
#include <component/base-component/VisualObject.h>
namespace MotionByte
{
class AbstractSlider : public VisualObject {
public:
    enum class DragOrientation
    {
        Horizontal,
        Vertical,
        HorizontalAndVertical
    };
    enum class DragType
    {
        Drag,
        Click
    };
public:
    AbstractSlider(float minValue, float maxValue, float value);
    void setValue(float value);
    float getValue();
    void setMinValue(float minValue);
    float getMinValue();
    void setMaxValue(float maxValue);
    float getMaxValue();
    // step 0 mean linear slider
    void setStep(int step);
    int getStep();
    void setRange(float minValue, float maxValue);
    void setRange(float minValue, float maxValue, float value);
    void setDragOrientation(DragOrientation orientation);
    DragOrientation getDragOrientation();
    void setDragType(DragType type);
    DragType getDragType();  
    void onPressed(Point point) override;
    void onMouseDragged(Point point, Point origin) override;
protected:
    Property mPercent;
private:
    void setPercent(float percent);
    void updateValue();
    virtual void updateVisualWithPercent(float percent);
    virtual void onValueChanged(float value);
    float mMinValue;
    float mMaxValue;
    float mValue;
    float mValueAtPress;
    
    int mStep;
    DragOrientation mDragOrientation;
    DragType mDragType;
};
}