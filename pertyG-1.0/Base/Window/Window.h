#pragma once
#include <GLFW/glfw3.h>
#include "GlwfManager.h"

#include <atomic>
#include <queue>
#include <functional>
#include <thread>

#include "../FrameRenderer/FrameRenderer.h"
#include "../Property/Property.h"
#include "../Property/PropertyManager.h"
namespace pertyG
{
    class Window : public IFrameEventListener
    {
    private:
        std::atomic<GLFWwindow*> mMainWindow;
        PropertyManager mPropertyManager;
        std::mutex mTaskListLocker;
        std::queue<std::function<void()>> mTaskList;
        Property size[2];
        bool mNeedUpdate = true;
        std::thread renderThread;
    public:
        enum PropertyList
        {
            WindowWidth,
            WindowHeight,
            PropertyCount
        };
        Window(int width, int height) : mPropertyManager(PropertyManager(PropertyCount))
        {
            mMainWindow = nullptr;
            create(width, height, "hello");
            mPropertyManager.initValue(WindowWidth, width);
            mPropertyManager.initValue(WindowHeight, height);
        }
        void addTask(std::function<void()> task)
        {
            std::lock_guard<std::mutex> lock(mTaskListLocker);
            mTaskList.push(task);
        }
        void handleWindow()
        {
            // Make the window's context current
            glfwMakeContextCurrent(mMainWindow);

            // Enter the rendering loop in a separate thread
            while (!glfwWindowShouldClose(mMainWindow))
            {

                // Your rendering code here
                {
                    std::lock_guard<std::mutex> lock(mTaskListLocker);
                    for (; !mTaskList.empty(); mTaskList.pop())
                    {
                        mTaskList.front()();
                    }
                }
                // Swap front and back buffers
                glfwSwapBuffers(mMainWindow);

                // Poll for and process events
                glfwPollEvents();
            }
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

                
            }
        }

         void show()
        {
            if (mMainWindow)
            {
                handleWindow();
            }
        }

        void setSize(int width, int height)
        {
            mPropertyManager.setValue(WindowWidth, width);
            mPropertyManager.setValue(WindowHeight, height);
            mNeedUpdate = !mPropertyManager.isSet();
        }
        void onFrameInitialized() override
        {
            int a = 2;
        }
        void onFrameProcessed() override
        {
            if (mMainWindow)
            {
                if (mNeedUpdate)
                {
                    std::cout << "add task" << std::endl;
                    addTask([this]
                        {
                            glfwSetWindowSize(mMainWindow,
                                mPropertyManager.getValue(WindowWidth),
                                mPropertyManager.getValue(WindowHeight));
                        });
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
        PropertyManager& getPropertyManager()
        {
            return mPropertyManager;
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
