#pragma once
#include "component/abstract-component/AbstractToggleButton.h"
#define DEFAULT_TICK_COLOR Color(240, 240, 240, 255)
#define DEFAULT_TICK_SIZE 10
#define DEFAULT_CHECK_BOX_SIZE 25
#define DEFAULT_THICKNESS 4
namespace MotionByte
{
    class CheckBox : public AbstractToggleButton
    {
    private:
        Color mInsideTickColor;
        Property mInsideTickSizeProperty;
        double mInsideTickSize;
        std::shared_ptr<VisualObject> mInsideTick;
    public:
        CheckBox();
        ~CheckBox();
        virtual void onButtonStateChanged(ButtonState state) override;
        Color &getInsideTickColor();
        void setInsideTickColor(Color insideTickColor);
        double getInsideTickSize();
        void setInsideTickSize(double insideTickSize);
        void paint(Frame &frame) override;

    };
}