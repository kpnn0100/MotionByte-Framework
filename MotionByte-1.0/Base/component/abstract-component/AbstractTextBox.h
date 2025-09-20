#pragma once
#include <component/base-component/VisualObject.h>
#include <component/display-component/Label.h>
#include <component/base-component/event/KeyboardEventManager.h>
#include <queue>
namespace MotionByte
{
    #define DEFAULT_BACKGROUND_COLOR Color(120, 120, 120)
    #define DEFAULT_BORDER_COLOR Color(0, 0, 0)
    #define DEFAULT_TEXT_COLOR Color(255, 255, 255)
    #define DEFAULT_HINT_COLOR Color(100, 100, 100)
    #define DEFAULT_CURSOR_COLOR Color(0, 0, 0)

    class AbstractTextBox : public KeyboardEventListener
                        , public Label
    {
    protected:

        std::queue<std::string> mHistory;
        std::string mHint;
        std::shared_ptr<VisualObject> mCursor;
        ColorManager mTextBoxColorList;
        PropertyManager mTextBoxPropertyList;
        Color mHintColor;
        Color mCursorColor;
        bool mIsPassword;
        bool mIsEditable;
        bool mIsSelecting;
        bool mIsSelectable;
        bool mIsFocused = false;
        int mCursorIndex;
        int mSelectionStart;
        int mSelectionEnd;
        int mMaxLength;
        std::string mAllowedCharacters;
        std::string mClipboard;
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
        void onClicked(Point point) override;

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

        void setFontSize(double size);
        double getFontSize();
        Property& getFontSizeProperty();

        int getCursorPosition();
        double getCursorXPosition();

        // Text editing operations
        void deleteSelection();
        void backspaceChar();
        void deleteChar();
        void selectAll();
        void copy();
        void paste();
        void undo();
        void redo();
        
        // Selection management
        void clearSelection();
        bool hasSelection() const;
        std::string getSelectedText() const;
        void setSelection(int start, int end);
        
        // Cursor movement
        void moveCursorLeft();
        void moveCursorRight();
        void moveCursorHome();
        void moveCursorEnd();
        void moveCursorWordLeft();
        void moveCursorWordRight();
        
        // Input validation
        void setMaxLength(int maxLength);
        int getMaxLength() const;
        void setAllowedCharacters(const std::string& allowedChars);
        std::string getAllowedCharacters() const;
        
        // Password support
        std::string getDisplayText() const;
        
        // Modifier key tracking
        void handleCtrlCharacterInput(char character);

        void paint(Frame& frame) override;
    };
}
