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
        std::function<void(ButtonState)> mStateCallback;
        ButtonState state = ButtonState::Released;
    public:
    
        virtual void onButtonStateChanged(ButtonState state);
        void setStateCallback(std::function<void(ButtonState)> stateCallback);
        void setButtonState(ButtonState state);
        ButtonState getButtonState();

        AbstractButton();
        ~AbstractButton();
    };
}


