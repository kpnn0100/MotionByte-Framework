#include "Slider.h"
namespace MotionByte
{
    Slider::Slider(float minValue, float maxValue, float value) : AbstractSlider(minValue, maxValue, value)
    {
        mSliderColor = DEFAULT_SLIDER_COLOR;
        mThumbColor = DEFAULT_THUMB_COLOR;
        mSegmentTypeName = "Slider";
        mBound.setSize(DEFAULT_SLIDER_BOUND_WIDTH, DEFAULT_SLIDER_BOUND_HEIGHT);
        mThumbRadius.initValue(DEFAULT_THUMB_RADIUS);
        mSliderHeight.initValue(DEFAULT_SLIDER_HEIGHT);
    }
    void Slider::paint(Frame &frame)
    {
        Rectangle fillShape = getLocalBound().withSizeKeepCenter(getBound().getWidth(),mSliderHeight);
        frame.fillRectangle(mSliderColor,fillShape);
        Point thumbPosition = Point(mBound.getWidth() * mPercent, mBound.getHeight()/2);
        frame.fillCircle(mThumbColor, thumbPosition, mThumbRadius );
    }
    void Slider::updateVisualWithPercent(float value)
    {
        
    }
}