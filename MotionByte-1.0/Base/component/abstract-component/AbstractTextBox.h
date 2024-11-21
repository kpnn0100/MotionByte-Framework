#pragma once
#include <component/base-component/VisualObject.h>
#include <component/display-component/Label.h>
#include <component/base-component/event/KeyboardEventManager.h>
#include <queue>
namespace MotionByte
{
    #define DEFAULT_BACKGROUND_COLOR Color(255, 255, 255)
    #define DEFAULT_BORDER_COLOR Color(0, 0, 0)
    #define DEFAULT_TEXT_COLOR Color(0, 0, 0)
    #define DEFAULT_HINT_COLOR Color(200, 200, 200)
    #define DEFAULT_CURSOR_COLOR Color(0, 0, 0)

    class AbstractTextBox : public VisualObject
                        , public KeyboardEventListener
    {
    protected:

        std::string mText;
        std::queue<std::string> mHistory;
        std::string mHint;
        std::shared_ptr<Label> mMainText;
        std::shared_ptr<Label> mHintText; 
        std::shared_ptr<VisualObject> mCursor;
        ColorManager mTextBoxColorList;
        PropertyManager mTextBoxPropertyList;
        bool mIsPassword;
        bool mIsEditable;
        bool mIsSelecting;
        bool mIsSelectable;
        bool mIsFocused = false;
        int mCursorIndex;
        int mSelectionStart;
        int mSelectionEnd;
    public:
        enum TextBoxColorID {
            Background = 0,
            Border,
            Text,
            Cursor,
            TextBoxColorCount
        };
        enum TextBoxPropertyID {
            TextSize,

            TextBoxPropertyCount
        };

        static const double DEFAULT_TEXT_SIZE;
        static const char* DEFAULT_TEXT;
        static const char* DEFAULT_HINT;
        static const double DEFAULT_WIDTH;
        static const double DEFAULT_HEIGHT;

        AbstractTextBox();
        ~AbstractTextBox();
        void onInput(FunctionKeyCode keyCode, KeyAction action) override;
        void onInput(char character, KeyAction action) override;
        void onInput(const std::string &text) override;
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

        void paint(Frame& frame) override;
    };
}
