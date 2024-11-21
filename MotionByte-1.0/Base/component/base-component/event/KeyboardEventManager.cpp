#include "KeyboardEventManager.h"
#include <debugging_tool/DEBUG.h>

namespace MotionByte
{

    KeyboardEventManager &KeyboardEventManager::getInstance()
    {
        static KeyboardEventManager instance;
        return instance;
    }

    KeyboardEventManager::KeyboardEventManager() {}

    void KeyboardEventManager::triggerInput(const std::string &text)
    {
        notifyInput(text);
        debug(DebugLevel::Debug, "Pasted text: ", text);
    }

    void KeyboardEventManager::triggerInput(FunctionKeyCode keyCode, KeyAction action)
    {
        notifyInput(keyCode, action);
    }

    void KeyboardEventManager::triggerInput(char character, KeyAction action)
    {
        debug(DebugLevel::Debug, "Char: ", character, " ", (char)character, " action: ", action == KeyAction::Pressed ? "Pressed" : "Released");
        notifyInput(character, action);
    }

    void KeyboardEventManager::notifyInput(const std::string &text)
    {
        for (auto *listener : mListenerList)
        {
            listener->onInput(text);
        }
    }

    void KeyboardEventManager::notifyInput(FunctionKeyCode keyCode, KeyAction action)
    {
        for (auto *listener : mListenerList)
        {
            listener->onInput(keyCode, action);
        }
    }

    void KeyboardEventManager::notifyInput(char character, KeyAction action)
    {
        for (auto *listener : mListenerList)
        {
            listener->onInput(character, action);
        }
    }

    void KeyboardEventManager::addListener(KeyboardEventListener *listener)
    {
        mListenerList.push_back(listener);
    }

    void KeyboardEventManager::removeListener(KeyboardEventListener *listener)
    {
        mListenerList.erase(std::remove(mListenerList.begin(), mListenerList.end(), listener), mListenerList.end());
    }
    // Callbacks
    void paste_callback(const std::string &pasteString)
    {
        KeyboardEventManager::getInstance().triggerInput(pasteString);

    }

    void char_callback(GLFWwindow *window, unsigned int codepoint)
    {
        KeyboardEventManager::getInstance().triggerInput(codepoint, KeyAction::Pressed);
        
    }

    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        KeyAction keyAction = (action == GLFW_PRESS || action == GLFW_REPEAT) ? KeyAction::Pressed : KeyAction::Released;
        if (key == GLFW_KEY_BACKSPACE)
        {
            KeyboardEventManager::getInstance().triggerInput(FunctionKeyCode::Backspace, keyAction);
        }
        else if (key == GLFW_KEY_ENTER)
        {
            KeyboardEventManager::getInstance().triggerInput(FunctionKeyCode::Enter, keyAction);
        }
        else if (action == GLFW_PRESS)
        {
            if (key == GLFW_KEY_V && (mods & GLFW_MOD_CONTROL))
            {
                const char *clipboardText = glfwGetClipboardString(window);
                if (clipboardText != nullptr)
                {
                    paste_callback(std::string(clipboardText));
                }
            }
        }
    }

    void KeyboardEventListener::onInput(FunctionKeyCode keyCode, KeyAction action) {}

    void KeyboardEventListener::onInput(char character, KeyAction action) {}

    void KeyboardEventListener::onInput(const std::string &text) {}

    } // namespace MotionByte
