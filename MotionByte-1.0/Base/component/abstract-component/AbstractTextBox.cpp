#include <component/abstract-component/AbstractTextBox.h>
#include "AbstractTextBox.h"
namespace MotionByte
{
    void AbstractTextBox::setText(const std::string &text) {
        mText = text;
        mHistory.push(text);
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
}