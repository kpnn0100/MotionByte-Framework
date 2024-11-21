#include "Label.h"
namespace MotionByte
{
    const double Label::DEFAULT_TEXT_SIZE = 12.0;
    const Color Label::DEFAULT_TEXT_COLOR = Color(255, 255, 255, 255);
    const Align Label::DEFAULT_TEXT_ALIGNMENT = Align::CenterMiddle;
    const double Label::DEFAULT_WIDTH = 100.0;
    const double Label::DEFAULT_HEIGHT = 20.0;
    Label::Label() : VisualObject()
    {
        mSegmentTypeName = "Label";
        mTextColor.setColor(Color(255, 255, 255, 255));
        mTextSize = DEFAULT_TEXT_SIZE;
        mTextSize.setMin(0.0);
        mBound.setSize(DEFAULT_WIDTH,DEFAULT_HEIGHT);
        mFont = FontManager::instance().createDefaultFont();
        
    }
    Label::Label(std::string text) : Label()
    {
        mText = text;
    }
    void Label::setFont(std::string fontPath)
    {
        mFont = FontManager::instance().createFont(fontPath);
    }
    std::shared_ptr<Font> Label::getFont()
    {
        return mFont;
    }
    void Label::setAlignment(Align alignment)
    {
        mTextAlignment = alignment;
    }
    void Label::onParentChanged()
    {
        bindBoundToParent();
    }
    void Label::paint(Frame &frame)
    {
        frame.drawText(mTextColor, *mFont, mText, mTextSize.getValue(), mBound, mTextAlignment);
    }
    void Label::setText(std::string text)
    {
        mText = text;
    }
    void Label::setTextColor(Color color)
    {
        mTextColor.setColor(color);
    }
    void Label::setTextSize(double size)
    {
        mTextSize = size;
    }
    std::string Label::getText()
    {
        return mText;
    }
    Color &Label::getTextColor()
    {
        return mTextColor;
    }
    Property &Label::getTextSize()
    {
        return mTextSize;
    }
} // namespace MotionByte