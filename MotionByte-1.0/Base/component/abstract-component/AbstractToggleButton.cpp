#include "AbstractToggleButton.h"
namespace MotionByte
{
    void AbstractToggleButton::onClicked(Point point)
    {
        if (state == ButtonState::Released)
        {
            setButtonState(ButtonState::Pressed);
        }
        else
        {
            setButtonState(ButtonState::Released);
        }
    }

    AbstractToggleButton::AbstractToggleButton()
    {
    }

    AbstractToggleButton::~AbstractToggleButton()
    {
    }

}
