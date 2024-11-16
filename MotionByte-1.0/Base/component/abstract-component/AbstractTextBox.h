#pragma once
#include <component/base-component/VisualObject.h>
#include <queue>
namespace MotionByte
{
    class AbstractTextBox : public VisualObject {
    protected:
        std::string mText;
        std::queue<std::string> mHistory;
        std::string mHint;
        bool mIsPassword;
        bool mIsEditable;
        bool mIsSelecting;
        bool mIsSelectable;
        bool mIsFocused = false;
        int mCursorIndex;
        int mSelectionStart;
        int mSelectionEnd;
    public:
        void onClickedOutside(Point point) override;

        void setText(const std::string &text);
        std::string getText();

        void setHint(const std::string &hint);
        std::string getHint();

        void setIsSelectable(bool isSelectable);
        bool getIsSelectable();

        void setIsPassword(bool isPassword);
        bool getIsPassword();

        void setIsEditable(bool isEditable);
        bool getIsEditable();

        void setCursorIndex(int cursorIndex);
        int getCursorIndex();

        void setSelectionStart(int selectionStart);
        int getSelectionStart();

        void setSelectionEnd(int selectionEnd);
        int getSelectionEnd();
    };
}
