#pragma once
#include <vector>
#include <string>
#include <GLFW/glfw3.h>

namespace MotionByte {

enum class KeyAction {
    Pressed = 0,
    Released
};

enum class FunctionKeyCode {
    Backspace = 0,
    Enter,
    Space,
    Tab,
    Shift,
    Control,
    Alt,
    CapsLock,
    Escape,
    PageUp,
    PageDown,
    End,
    Home,
    Left,
    Up,
    Right,
    Down,
    Insert,
    Delete,
    NumLock,
    ScrollLock,
    PrintScreen,
    Pause,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
    Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6,
    Keypad7, Keypad8, Keypad9, KeypadDecimal, KeypadDivide, KeypadMultiply,
    KeypadSubtract, KeypadAdd, KeypadEnter, KeypadEqual, LeftShift,
    LeftControl, LeftAlt, RightShift, RightControl, RightAlt, RightSuper,
    Menu, Last
};

class KeyboardEventListener {
public:
    virtual void onInput(FunctionKeyCode keyCode, KeyAction action);
    virtual void onInput(char character, KeyAction action);
    virtual void onInput(const std::string &text);
    virtual ~KeyboardEventListener() = default;
};

class KeyboardEventManager {
public:
    static KeyboardEventManager& getInstance();

    void triggerInput(const std::string& text);
    void triggerInput(FunctionKeyCode keyCode, KeyAction action);
    void triggerInput(char character, KeyAction action);

    void addListener(KeyboardEventListener* listener);
    void removeListener(KeyboardEventListener* listener);

private:
    KeyboardEventManager();
    void notifyInput(const std::string& text);
    void notifyInput(FunctionKeyCode keyCode, KeyAction action);
    void notifyInput(char character, KeyAction action);

    std::vector<KeyboardEventListener*> mListenerList;
};

// Callbacks
void paste_callback(GLFWwindow* window);
void char_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

} // namespace MotionByte
