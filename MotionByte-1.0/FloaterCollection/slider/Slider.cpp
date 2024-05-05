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
        mThumbShadowSize.initValue(0);
        mThumbShadowSize.setInterpolator(InterpolatorFactory::createSmooth(10000.0, 500.0));
    }
    void Slider::paint(Frame &frame)
    {
        Rectangle fillShape = getLocalBound();
        Rectangle progressShape = getLocalBound();
        double radius = fillShape.getHeight()/2;
        frame.drawAnnularArc(mSliderColor,Point(radius,radius),0,radius,-90,90,Frame::ClockWise);
        progressShape.setPosition(Point(radius,0));
        progressShape.setSize((fillShape.getWidth()-radius*2) * mPercent , fillShape.getHeight());
        frame.fillRectangle(mSliderColor,progressShape);
        frame.drawRoundedRectangle(mSliderColor,fillShape,radius, mThumbRadius/4);
        Point thumbPosition = Point((mBound.getWidth()- radius*2) * mPercent+radius, mBound.getHeight()/2);
        frame.fillCircle(mThumbColor.withAlpha(0.5), thumbPosition, mThumbShadowSize);
        frame.fillCircle(mThumbColor, thumbPosition, mThumbRadius );
    }
    void Slider::onMouseFocused()
    {
        AbstractSlider::onMouseFocused();
        mThumbShadowSize = mThumbRadius.getValue()*2;
    }
    void Slider::onMouseUnfocused()
    {
        AbstractSlider::onMouseUnfocused();
        mThumbShadowSize = 0;
    }
    void Slider::updateVisualWithPercent(float value)
    {
        
    }
}