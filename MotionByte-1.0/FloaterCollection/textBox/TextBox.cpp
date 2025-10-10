#include "TextBox.h"

namespace MotionByte
{
    void TextBox::paint(Frame& frame)
    {
        // Draw Text
        std::string displayText = getDisplayText();
        if (displayText.empty() && !mIsFocused) {
            // Draw hint text
            frame.drawText(mTextBoxColorList.getColor(TextBoxColorID::Text), *mFont, mHint, mTextSize.getValue(), getLocalBound(), mTextAlignment);
        } else {
            frame.drawText(mTextBoxColorList.getColor(TextBoxColorID::Text), *mFont, displayText, mTextSize.getValue(), getLocalBound(), mTextAlignment);
        }
        
    }
}