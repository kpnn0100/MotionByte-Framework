#pragma once
#include "AbstractButton.h"
namespace MotionByte
{
    class AbstractPushButton : public AbstractButton
    {
        public:
            void onPressed(Point point) override;
            void onReleased(Point point) override;
            AbstractPushButton();
            ~AbstractPushButton();
    };
}
