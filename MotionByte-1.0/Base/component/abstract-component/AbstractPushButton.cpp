#include "AbstractPushButton.h"
namespace MotionByte
{
    void AbstractPushButton::onPressed(Point point)
    {
        setButtonState(ButtonState::Pressed);
    }
    void AbstractPushButton::onReleased(Point point)
    {
        setButtonState(ButtonState::Released);
    } 
    AbstractPushButton::AbstractPushButton()
    {
    }

    AbstractPushButton::~AbstractPushButton()
    {
    }

}