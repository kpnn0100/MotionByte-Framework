#pragma once
#include "AbstractButton.h"
namespace MotionByte
{
    class AbstractToggleButton : public AbstractButton
    {
    public:
        void onClicked(Point point) override;
        AbstractToggleButton();
        ~AbstractToggleButton();
    };
}