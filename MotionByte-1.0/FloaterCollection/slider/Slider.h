#pragma once
#include "component/abstract-component/AbstractSlider.h"
#define DEFAULT_MIN_VALUE 0
#define DEFAULT_MAX_VALUE 100
#define DEFAULT_VALUE 50
#define DEFAULT_SLIDER_HEIGHT 5
#define DEFAULT_SLIDER_COLOR Color(220, 220, 220, 255)
#define DEFAULT_THUMB_COLOR Color(255, 255, 255, 255)
#define DEFAULT_THUMB_RADIUS 10
#define DEFAULT_SLIDER_BOUND_WIDTH 100
#define DEFAULT_SLIDER_BOUND_HEIGHT 20
namespace MotionByte
{
    class Slider : public AbstractSlider
    {
    protected:
        Color mSliderColor;
        Color mThumbColor;
        Property mThumbRadius;
        Property mSliderHeight;
        Property mThumbShadowSize;
        void paint(Frame &frame) override;
    public:
        void onMouseEntered() override;
        void onMouseExited() override;
        Slider(float minValue, float maxValue, float value);
        void updateVisualWithPercent(float value) override;

    };
}