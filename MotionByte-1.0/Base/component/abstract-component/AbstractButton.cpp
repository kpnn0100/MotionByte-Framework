#include "AbstractButton.h"
namespace MotionByte
{
    AbstractButton::AbstractButton()
    {
    }

    AbstractButton::~AbstractButton()
    {
    }

    void AbstractButton::onButtonStateChanged(ButtonState state)
    {

    }

    void AbstractButton::setStateCallback(std::function<void(ButtonState)> stateCallback)
    {
        mStateCallback = stateCallback;
    }

    void AbstractButton::setButtonState(ButtonState state)
    {
        if (state != this->state)
        {
            this->state = state;
            onButtonStateChanged(state);
            mStateCallback(state);
        }
    }
    AbstractButton::ButtonState AbstractButton::getButtonState()
    {
        return state;
    }
}
