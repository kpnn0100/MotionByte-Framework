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

    void AbstractButton::setButtonState(ButtonState state)
    {
        if (state != this->state)
        {
            this->state = state;
            onButtonStateChanged(state);
        }
    }
    AbstractButton::ButtonState AbstractButton::getButtonState()
    {
        return state;
    }
}
