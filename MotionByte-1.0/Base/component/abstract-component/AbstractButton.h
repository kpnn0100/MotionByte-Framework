#pragma once
#include "component/display-component/Label.h"
namespace MotionByte
{
    class AbstractButton : public VisualObject
    {
    public:
        enum class ButtonState
        {
            Released = 0,
            Pressed
        };
    protected:
        ButtonState state = ButtonState::Released;
    public:
        virtual void onButtonStateChanged(ButtonState state);
        void setButtonState(ButtonState state);
        ButtonState getButtonState();

        AbstractButton();
        ~AbstractButton();
    };
}


