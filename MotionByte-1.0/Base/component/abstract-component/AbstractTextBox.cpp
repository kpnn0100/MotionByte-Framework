#include <component/abstract-component/AbstractTextBox.h>
#include "AbstractTextBox.h"
#include <cctype>
#include <algorithm>
namespace MotionByte
{

    const double AbstractTextBox::DEFAULT_TEXT_SIZE = 24.0;
    const char* AbstractTextBox::DEFAULT_TEXT = "";
    const char* AbstractTextBox::DEFAULT_HINT = "Enter text...";
    const double AbstractTextBox::DEFAULT_WIDTH = 100.0;
    const double AbstractTextBox::DEFAULT_HEIGHT = 30.0;

    void AbstractTextBox::setText(const std::string &text) {
        mText = text;
        mHistory.push(text);
    }

    AbstractTextBox::AbstractTextBox() : mTextBoxColorList(TextBoxColorID::TextBoxColorCount), mTextBoxPropertyList(TextBoxPropertyCount)
    {
        mSegmentTypeName = "AbstractTextBox";
        mCursorIndex = 0;
        mSelectionStart = -1;
        mSelectionEnd = -1;
        mIsSelecting = false;
        mIsSelectable = true;
        mMaxLength = -1; // No limit by default
        mAllowedCharacters = ""; // All characters allowed by default
        
        setBackgroundColor(DEFAULT_BACKGROUND_COLOR);
        setTextColor(DEFAULT_TEXT_COLOR);

        setBound(Rectangle(0,0,DEFAULT_WIDTH, DEFAULT_HEIGHT));
        mTextBoxColorList.setColorPointer(TextBoxColorID::Background, &getObjectColors().getColor(VisualObject::ObjectColorID::Background));
        mTextBoxColorList.setColorPointer(TextBoxColorID::Border, &getObjectColors().getColor(VisualObject::ObjectColorID::Border));
        mTextBoxColorList.setColorPointer(TextBoxColorID::Text, &mTextColor);
        mTextBoxColorList.setColorPointer(TextBoxColorID::Cursor, &mCursorColor);
        mTextBoxColorList.getColor(TextBoxColorID::Border).setColorName("TextBox::BorderColor");
        mTextBoxColorList.getColor(TextBoxColorID::Background).setColorName("TextBox::BackgroundColor");
        mTextBoxColorList.getColor(TextBoxColorID::Text).setColorName("TextBox::TextColor");
        mTextBoxColorList.getColor(TextBoxColorID::Cursor).setColorName("TextBox::CursorColor");
        mTextAlignment = Align::CenterLeft;
        mTextBoxPropertyList.setPropertyPointer(TextBoxPropertyID::TextSize, &mTextSize);
        mTextBoxPropertyList.getProperty(TextBoxPropertyID::TextSize).setPropertyName("TextBox::TextSize");
        setText(DEFAULT_TEXT);
        setFontSize(DEFAULT_TEXT_SIZE);
        setHint(DEFAULT_HINT);
        mIsEditable = true;
    }
    AbstractTextBox::~AbstractTextBox()
    {
        
    }

    void AbstractTextBox::onInput(FunctionKeyCode keyCode, KeyAction action)
    {
        if (!mIsFocused) return;
        
        // Track modifier keys
        static bool ctrlPressed = false;
        static bool shiftPressed = false;
        
        if (keyCode == FunctionKeyCode::Control || keyCode == FunctionKeyCode::LeftControl) {
            ctrlPressed = (action == KeyAction::Pressed);
            return;
        }
        
        if (keyCode == FunctionKeyCode::Shift || keyCode == FunctionKeyCode::LeftShift) {
            shiftPressed = (action == KeyAction::Pressed);
            return;
        }
        
        if (action != KeyAction::Pressed) return;
        
        // Handle Ctrl+key combinations
        if (ctrlPressed) {
            switch (keyCode) {
                case FunctionKeyCode::Left:
                    if (shiftPressed) {
                        // Ctrl+Shift+Left: Select word left (simplified implementation)
                        int oldCursor = mCursorIndex;
                        moveCursorWordLeft();
                        if (mSelectionStart == -1) {
                            mSelectionStart = oldCursor;
                            mSelectionEnd = mCursorIndex;
                        } else {
                            mSelectionEnd = mCursorIndex;
                        }
                    } else {
                        moveCursorWordLeft();
                    }
                    break;
                    
                case FunctionKeyCode::Right:
                    if (shiftPressed) {
                        // Ctrl+Shift+Right: Select word right (simplified implementation)
                        int oldCursor = mCursorIndex;
                        moveCursorWordRight();
                        if (mSelectionStart == -1) {
                            mSelectionStart = oldCursor;
                            mSelectionEnd = mCursorIndex;
                        } else {
                            mSelectionEnd = mCursorIndex;
                        }
                    } else {
                        moveCursorWordRight();
                    }
                    break;
                    
                default:
                    debug(DebugLevel::Verbose, "Unhandled Ctrl+key combination");
                    break;
            }
            return;
        }
        
        // Handle Shift+key combinations for selection
        if (shiftPressed) {
            int oldCursor = mCursorIndex;
            bool moved = false;
            
            switch (keyCode) {
                case FunctionKeyCode::Left:
                    if (mCursorIndex > 0) {
                        mCursorIndex--;
                        moved = true;
                    }
                    break;
                    
                case FunctionKeyCode::Right:
                    if (mCursorIndex < static_cast<int>(mText.length())) {
                        mCursorIndex++;
                        moved = true;
                    }
                    break;
                    
                case FunctionKeyCode::Home:
                    mCursorIndex = 0;
                    moved = true;
                    break;
                    
                case FunctionKeyCode::End:
                    mCursorIndex = static_cast<int>(mText.length());
                    moved = true;
                    break;
            }
            
            if (moved && mIsSelectable) {
                if (mSelectionStart == -1) {
                    mSelectionStart = oldCursor;
                    mSelectionEnd = mCursorIndex;
                } else {
                    mSelectionEnd = mCursorIndex;
                }
            }
            return;
        }
        
        // Handle regular key presses
        switch (keyCode) {
            case FunctionKeyCode::Backspace:
                if (mIsEditable) {
                    backspaceChar();
                }
                break;
                
            case FunctionKeyCode::Delete:
                if (mIsEditable) {
                    deleteChar();
                }
                break;
                
            case FunctionKeyCode::Left:
                moveCursorLeft();
                break;
                
            case FunctionKeyCode::Right:
                moveCursorRight();
                break;
                
            case FunctionKeyCode::Home:
                moveCursorHome();
                break;
                
            case FunctionKeyCode::End:
                moveCursorEnd();
                break;
                
            case FunctionKeyCode::Enter:
                // Could trigger onEnter callback if implemented
                debug(DebugLevel::Verbose, "Enter pressed in text box");
                break;
                
            case FunctionKeyCode::Tab:
                // Could handle tab navigation if implemented
                debug(DebugLevel::Verbose, "Tab pressed in text box");
                break;
                
            case FunctionKeyCode::Escape:
                mIsFocused = false;
                clearSelection();
                break;
                
            default:
                break;
        }
    }

    void AbstractTextBox::onInput(char character, KeyAction action)
    {
        if (action != KeyAction::Pressed || !mIsFocused) return;
        
        // Handle Ctrl+character combinations (characters 1-26 represent Ctrl+A through Ctrl+Z)
        if (character >= 1 && character <= 26) {
            handleCtrlCharacterInput(character);
            return;
        }
        
        if (!mIsEditable) return;
        
        // Input validation
        if (!mAllowedCharacters.empty() && mAllowedCharacters.find(character) == std::string::npos) {
            debug(DebugLevel::Verbose, "Character not allowed: ", character);
            return;
        }
        
        // Check max length (if set)
        if (mMaxLength > 0 && static_cast<int>(mText.length()) >= mMaxLength && !hasSelection()) {
            debug(DebugLevel::Verbose, "Max length reached");
            return;
        }
        
        // Delete selection if any
        if (hasSelection()) {
            deleteSelection();
        }
        
        // Insert character
        mText.insert(static_cast<size_t>(mCursorIndex), 1, character);
        mCursorIndex++;
        setText(mText);
        
        debug(DebugLevel::Verbose, "Input char: ", character, " at index ", mCursorIndex);
        debug(DebugLevel::Verbose, "Current text: ", mText);
    }
    void AbstractTextBox::onInput(const std::string &text)
    {
        if (!mIsEditable || !mIsFocused || text.empty()) return;
        
        // Validate input text
        std::string validText = text;
        if (!mAllowedCharacters.empty()) {
            std::string filtered;
            for (char c : text) {
                if (mAllowedCharacters.find(c) != std::string::npos) {
                    filtered += c;
                }
            }
            validText = filtered;
        }
        
        if (validText.empty()) return;
        
        // Check max length
        if (mMaxLength > 0) {
            int availableSpace = mMaxLength - static_cast<int>(mText.length());
            if (hasSelection()) {
                availableSpace += static_cast<int>(getSelectedText().length());
            }
            if (availableSpace <= 0) return;
            
            if (static_cast<int>(validText.length()) > availableSpace) {
                validText = validText.substr(0, static_cast<size_t>(availableSpace));
            }
        }
        
        // Delete selection if any
        if (hasSelection()) {
            deleteSelection();
        }
        
        // Insert text
        mText.insert(static_cast<size_t>(mCursorIndex), validText);
        mCursorIndex += static_cast<int>(validText.length());
        setText(mText);
        
        debug(DebugLevel::Verbose, "Input text: ", validText, " at index ", mCursorIndex);
        debug(DebugLevel::Verbose, "Current text: ", mText);
    }
    void AbstractTextBox::onClickedOutside(Point point)
    {
        mIsFocused = false;
    }
    
    void AbstractTextBox::onClicked(Point point)
    {
        mIsFocused = true;
        clearSelection(); // Clear selection on new click
        
        debug(DebugLevel::Verbose, "TextBox focused at point: ", point.getX().getValue(), ", ", point.getY().getValue());
        
        // Find cursor position based on click point
        double clickX = point.getX().getValue();
        auto currentFont = getFont();
        
        // Null pointer check
        if (!currentFont) {
            mCursorIndex = 0;
            return;
        }
        
        // Handle empty text
        if (mText.empty()) {
            mCursorIndex = 0;
            return;
        }
        
        // If click is beyond text, place cursor at end
        double totalTextWidth = currentFont->getWidth(mText, getFontSize());
        if (clickX >= totalTextWidth) {
            mCursorIndex = static_cast<int>(mText.length());
            return;
        }
        
        // If click is before text, place cursor at beginning
        if (clickX <= 0) {
            mCursorIndex = 0;
            return;
        }
        
        // Binary search for the closest character index
        int left = 0;
        int right = static_cast<int>(mText.length());
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            std::string subText = mText.substr(0, static_cast<size_t>(mid));
            double textWidth = currentFont->getWidth(subText, getFontSize());
            
            if (mid < static_cast<int>(mText.length())) {
                std::string subText1 = mText.substr(0, static_cast<size_t>(mid + 1));
                double textWidth1 = currentFont->getWidth(subText1, getFontSize());
                
                if (textWidth <= clickX && textWidth1 > clickX) {
                    // Found the position between characters
                    // Snap to closest position
                    if (clickX - textWidth < textWidth1 - clickX) {
                        mCursorIndex = mid;
                    } else {
                        mCursorIndex = mid + 1;
                    }
                    return;
                }
            }
            
            if (textWidth < clickX) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        // Fallback - should not reach here with proper binary search
        mCursorIndex = left;
    }
    int AbstractTextBox::getCursorPosition() {
        return mCursorIndex;
    }

    double AbstractTextBox::getCursorXPosition() {
        int idx = mCursorIndex;
        if (idx < 0) idx = 0;
        if (idx > static_cast<int>(mText.size())) idx = static_cast<int>(mText.size());

        auto font = getFont();
        if (!font) return 0.0;

        // Use display text for proper cursor positioning (handles password masking)
        std::string displayText = getDisplayText();
        if (idx > static_cast<int>(displayText.size())) idx = static_cast<int>(displayText.size());
        
        std::string subText = displayText.substr(0, static_cast<size_t>(idx));
        return font->getWidth(subText, getFontSize());
    }
    double AbstractTextBox::getFontSize() {
        return mTextBoxPropertyList.getProperty(TextBoxPropertyID::TextSize).getValue();
    }
    Property& AbstractTextBox::getFontSizeProperty() {
        return mTextBoxPropertyList.getProperty(TextBoxPropertyID::TextSize);
    }
    void AbstractTextBox::setFontSize(double size) {
        mTextBoxPropertyList.getProperty(TextBoxPropertyID::TextSize) = size;
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
    void AbstractTextBox::paint(Frame &frame)
    {
    }

    // Text editing operations
    void AbstractTextBox::deleteSelection() {
        if (!hasSelection()) return;
        
        int start = std::min(mSelectionStart, mSelectionEnd);
        int end = std::max(mSelectionStart, mSelectionEnd);
        
        mText.erase(static_cast<size_t>(start), static_cast<size_t>(end - start));
        mCursorIndex = start;
        clearSelection();
        setText(mText);
    }

    void AbstractTextBox::backspaceChar() {
        if (hasSelection()) {
            deleteSelection();
            return;
        }
        
        if (mCursorIndex > 0) {
            mText.erase(static_cast<size_t>(mCursorIndex - 1), 1);
            mCursorIndex--;
            setText(mText);
        }
    }

    void AbstractTextBox::deleteChar() {
        if (hasSelection()) {
            deleteSelection();
            return;
        }
        
        if (mCursorIndex < static_cast<int>(mText.length())) {
            mText.erase(static_cast<size_t>(mCursorIndex), 1);
            setText(mText);
        }
    }

    void AbstractTextBox::selectAll() {
        if (!mIsSelectable || mText.empty()) return;
        
        mSelectionStart = 0;
        mSelectionEnd = static_cast<int>(mText.length());
        mCursorIndex = mSelectionEnd;
    }

    void AbstractTextBox::copy() {
        if (!hasSelection()) return;
        
        mClipboard = getSelectedText();
        debug(DebugLevel::Verbose, "Copied to clipboard: ", mClipboard);
    }

    void AbstractTextBox::paste() {
        if (!mIsEditable || mClipboard.empty()) return;
        
        onInput(mClipboard);
    }

    void AbstractTextBox::undo() {
        if (mHistory.empty()) return;
        
        std::string previousText = mHistory.front();
        mHistory.pop();
        mText = previousText;
        mCursorIndex = std::min(mCursorIndex, static_cast<int>(mText.length()));
        clearSelection();
        // Note: Don't call setText() here to avoid adding to history again
    }

    void AbstractTextBox::redo() {
        // Simple implementation - would need separate redo stack in full implementation
        debug(DebugLevel::Verbose, "Redo not implemented with full undo/redo stack");
    }

    // Selection management
    void AbstractTextBox::clearSelection() {
        mSelectionStart = -1;
        mSelectionEnd = -1;
        mIsSelecting = false;
    }

    bool AbstractTextBox::hasSelection() const {
        return mSelectionStart != -1 && mSelectionEnd != -1 && mSelectionStart != mSelectionEnd;
    }

    std::string AbstractTextBox::getSelectedText() const {
        if (!hasSelection()) return "";
        
        int start = std::min(mSelectionStart, mSelectionEnd);
        int end = std::max(mSelectionStart, mSelectionEnd);
        
        return mText.substr(static_cast<size_t>(start), static_cast<size_t>(end - start));
    }

    void AbstractTextBox::setSelection(int start, int end) {
        if (!mIsSelectable) return;
        
        start = std::max(0, std::min(start, static_cast<int>(mText.length())));
        end = std::max(0, std::min(end, static_cast<int>(mText.length())));
        
        mSelectionStart = start;
        mSelectionEnd = end;
        mCursorIndex = end;
    }

    // Cursor movement
    void AbstractTextBox::moveCursorLeft() {
        if (hasSelection()) {
            mCursorIndex = std::min(mSelectionStart, mSelectionEnd);
            clearSelection();
        } else if (mCursorIndex > 0) {
            mCursorIndex--;
        }
    }

    void AbstractTextBox::moveCursorRight() {
        if (hasSelection()) {
            mCursorIndex = std::max(mSelectionStart, mSelectionEnd);
            clearSelection();
        } else if (mCursorIndex < static_cast<int>(mText.length())) {
            mCursorIndex++;
        }
    }

    void AbstractTextBox::moveCursorHome() {
        mCursorIndex = 0;
        clearSelection();
    }

    void AbstractTextBox::moveCursorEnd() {
        mCursorIndex = static_cast<int>(mText.length());
        clearSelection();
    }

    void AbstractTextBox::moveCursorWordLeft() {
        if (mCursorIndex <= 0) return;
        
        // Skip current whitespace
        while (mCursorIndex > 0 && std::isspace(mText[static_cast<size_t>(mCursorIndex - 1)])) {
            mCursorIndex--;
        }
        
        // Skip to start of word
        while (mCursorIndex > 0 && !std::isspace(mText[static_cast<size_t>(mCursorIndex - 1)])) {
            mCursorIndex--;
        }
        
        clearSelection();
    }

    void AbstractTextBox::moveCursorWordRight() {
        if (mCursorIndex >= static_cast<int>(mText.length())) return;
        
        // Skip current word
        while (mCursorIndex < static_cast<int>(mText.length()) && !std::isspace(mText[static_cast<size_t>(mCursorIndex)])) {
            mCursorIndex++;
        }
        
        // Skip whitespace
        while (mCursorIndex < static_cast<int>(mText.length()) && std::isspace(mText[static_cast<size_t>(mCursorIndex)])) {
            mCursorIndex++;
        }
        
        clearSelection();
    }

    // Input validation
    void AbstractTextBox::setMaxLength(int maxLength) {
        mMaxLength = maxLength;
    }

    int AbstractTextBox::getMaxLength() const {
        return mMaxLength;
    }

    void AbstractTextBox::setAllowedCharacters(const std::string& allowedChars) {
        mAllowedCharacters = allowedChars;
    }

    std::string AbstractTextBox::getAllowedCharacters() const {
        return mAllowedCharacters;
    }

    // Password support
    std::string AbstractTextBox::getDisplayText() const {
        if (!mIsPassword) {
            return mText;
        }
        
        // Return masked text for password fields
        return std::string(mText.length(), '*');
    }
    
    // Modifier key tracking
    void AbstractTextBox::handleCtrlCharacterInput(char character) {
        switch (character) {
            case 1: // Ctrl+A
                selectAll();
                debug(DebugLevel::Verbose, "Ctrl+A: Select All");
                break;
                
            case 3: // Ctrl+C
                copy();
                debug(DebugLevel::Verbose, "Ctrl+C: Copy");
                break;
                
            case 22: // Ctrl+V
                paste();
                debug(DebugLevel::Verbose, "Ctrl+V: Paste");
                break;
                
            case 24: // Ctrl+X
                if (hasSelection()) {
                    copy();
                    deleteSelection();
                    debug(DebugLevel::Verbose, "Ctrl+X: Cut");
                }
                break;
                
            case 26: // Ctrl+Z
                undo();
                debug(DebugLevel::Verbose, "Ctrl+Z: Undo");
                break;
                
            default:
                debug(DebugLevel::Verbose, "Unhandled Ctrl+char: ", static_cast<int>(character));
                break;
        }
    }
}