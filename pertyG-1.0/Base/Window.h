#pragma once
#include <GLFW/glfw3.h>
#include "Property.h"
#include "GlwfManager.h"
#include "FrameRenderer.h"
namespace pertyG
{
    class Window : public IPropertyListener, public IFrameEventListener
    {
    private:
        GLFWwindow* mMainWindow;
        enum PropertyList
        {
            PropertyCount
        };
        Property size[2];
        bool mChanged = false;
        std::thread renderThread;
    public:
        Window()
        {
            mMainWindow = nullptr;
            size[0].setValue(800.0);  // Default width
            size[1].setValue(600.0);  // Default height
            create((int)size[0].getTargetValue(),(int)size[1].getTargetValue(), "hello");
        }

        void create(int width, int height, const char* title)
        {
            if (mMainWindow == nullptr)
            {
                if (!glfwInit())
                {
                    return;
                }

                // Set GLFW window hints (optional)
                // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

                mMainWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
                if (!mMainWindow)
                {
                    // Window creation failed
                    // You can add error handling or logging here
                    return;
                }

                // Make the window's context current
                glfwMakeContextCurrent(mMainWindow);
            }
        }

         void show()
        {
            if (mMainWindow)
            {
                renderThread = std::thread([this] {
                    // Enter the rendering loop in a separate thread
                    while (!glfwWindowShouldClose(mMainWindow))
                    {

                        // Your rendering code here

                        // Swap front and back buffers
                        glfwSwapBuffers(mMainWindow);

                        // Poll for and process events
                        glfwPollEvents();
                    }
                });
            }
        }

        void setSize(int width, int height)
        {
            size[0].setValue((double)width);
            size[1].setValue((double)height);
            mChanged = true;
        }
        void onPropertyChanged() override
        {

        }
        void onFrameProcessed() override
        {
            if (mMainWindow)
            {
                std::cout<<"set new Window size"<<std::endl;
                glfwSetWindowSize(mMainWindow, (int)size[0].getValue(), (int)size[1].getValue());
            }
        }
        void onFrameRendered() override
        {

        }
        void waitToClose()
        {
            if (renderThread.joinable())
            {
                renderThread.join();
            }
        }
        void close()
        {
            if (mMainWindow)
            {
                glfwSetWindowShouldClose(mMainWindow, GLFW_TRUE);
            }
            if (renderThread.joinable())
            {
                renderThread.join();
            }
        }

        ~Window()
        {
            if (mMainWindow)
            {
                glfwDestroyWindow(mMainWindow);
            }
        }
    };
}
