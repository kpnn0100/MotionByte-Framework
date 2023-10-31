#pragma once
#include <GLFW/glfw3.h>
#include "Property.h"
#include "PropertyManager.h"
#include "GlwfManager.h"
#include "FrameRenderer.h"
namespace pertyG
{
    class Window : public IFrameEventListener
    {
    private:
        GLFWwindow* mMainWindow;
        PropertyManager mPropertyManager;
        enum PropertyList
        {
            WindowWidth,
            WindowHeight,
            PropertyCount
        };
        Property size[2];
        bool mNeedUpdate = true;
        std::thread renderThread;
    public:
        Window() : mPropertyManager(PropertyManager(PropertyCount))
        {

            mMainWindow = nullptr;
            mPropertyManager.setValue(WindowWidth,800.0);  // Default width
            mPropertyManager.setValue(WindowHeight,600.0);  // Default height
            create((int)mPropertyManager.getTargetValue(WindowWidth),(int)mPropertyManager.getTargetValue(WindowHeight), "hello");
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
            mPropertyManager.setValue(WindowWidth,(double)width);
            mPropertyManager.setValue(WindowHeight,(double)height);
        }
        void onFrameProcessed() override
        {
            if (mMainWindow)
            {
                if (mNeedUpdate)
                {
                    std::cout<<"set new Window size"<<std::endl;
                    glfwSetWindowSize(mMainWindow, (int)mPropertyManager.getValue(WindowWidth), (int)mPropertyManager.getValue(WindowHeight));
                } 
            }
        }
        void onFrameRendered() override
        {
            mNeedUpdate = !mPropertyManager.isSet();
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
