#include <component/abstract-component/AbstractTextBox.h>
#include "AbstractTextBox.h"
namespace MotionByte
{

    const double AbstractTextBox::DEFAULT_TEXT_SIZE = 24.0;
    const char* AbstractTextBox::DEFAULT_TEXT = "adada";
    const char* AbstractTextBox::DEFAULT_HINT = "hint";
    const double AbstractTextBox::DEFAULT_WIDTH = 100.0;
    const double AbstractTextBox::DEFAULT_HEIGHT = 30.0;

    void AbstractTextBox::setText(const std::string &text) {
        mText = text;
        mMainText->setText(text);
        mHistory.push(text);
    }

    AbstractTextBox::AbstractTextBox() : mTextBoxColorList(TextBoxColorID::TextBoxColorCount), mTextBoxPropertyList(TextBoxPropertyCount)
    {
        mSegmentTypeName = "TextBox";
        mMainText = std::make_shared<Label>();
        mHintText = std::make_shared<Label>();
        mCursor = std::make_shared<VisualObject>();
        mMainText->setText(DEFAULT_TEXT);
        mHintText->setText(DEFAULT_HINT);
        
        mMainText->setTextColor(DEFAULT_TEXT_COLOR);
        mHintText->setTextColor(DEFAULT_HINT_COLOR);
        mMainText->setTextSize(DEFAULT_TEXT_SIZE);
        mHintText->setTextSize(DEFAULT_TEXT_SIZE);
        mMainText->setAlignment(Align::CenterLeft);
        mHintText->setAlignment(Align::CenterLeft);
        setBackgroundColor(DEFAULT_BACKGROUND_COLOR);
        setBound(Rectangle(0,0,DEFAULT_WIDTH, DEFAULT_HEIGHT));
        mIsEditable = true;
        addSegment(mMainText);
        addSegment(mHintText);
        mHintText->bindBoundToParent();
        mMainText->bindBoundToParent();
    }
    AbstractTextBox::~AbstractTextBox()
    {
        
    }
    void AbstractTextBox::onInput(FunctionKeyCode keyCode, KeyAction action)
    {
    }
    void AbstractTextBox::onInput(char character, KeyAction action)
    {
    }
    void AbstractTextBox::onInput(const std::string &text)
    {
    }
    void AbstractTextBox::onClickedOutside(Point point)
    {
        mIsFocused = false;
    }
    std::string AbstractTextBox::getText() {
        return mText;
    }

    void AbstractTextBox::setHint(const std::string &hint) {
        mHint = hint;
        mHintText->setText(hint);
    }

    std::string AbstractTextBox::getHint() {
        return mHint;
    }

    void AbstractTextBox::setIsSelectable(bool isSelectable) {
        mIsSelectable = isSelectable;
    }

    bool AbstractTextBox::getIsSelectable() {
        return mIsSelectable;
    }

    void AbstractTextBox::setIsPassword(bool isPassword) {
        mIsPassword = isPassword;
    }

    bool AbstractTextBox::getIsPassword() {
        return mIsPassword;
    }

    void AbstractTextBox::setIsEditable(bool isEditable) {
        mIsEditable = isEditable;
    }

    bool AbstractTextBox::getIsEditable() {
        return mIsEditable;
    }

    void AbstractTextBox::setCursorIndex(int cursorIndex) {
        mCursorIndex = cursorIndex;
    }

    int AbstractTextBox::getCursorIndex() {
        return mCursorIndex;
    }

    void AbstractTextBox::setSelectionStart(int selectionStart) {
        mSelectionStart = selectionStart;
    }

    int AbstractTextBox::getSelectionStart() {
        return mSelectionStart;
    }

    void AbstractTextBox::setSelectionEnd(int selectionEnd) {
        mSelectionEnd = selectionEnd;
    }

    int AbstractTextBox::getSelectionEnd() {
        return mSelectionEnd;
    }
    void AbstractTextBox::paint(Frame &frame)
    {
    }
}