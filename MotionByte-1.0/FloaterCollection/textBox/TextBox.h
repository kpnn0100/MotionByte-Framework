#pragma once
#include <component/abstract-component/AbstractTextBox.h>
namespace MotionByte
{
    class TextBox : public AbstractTextBox
    {
    public:
        void paint(Frame& frame) override;
    };
}