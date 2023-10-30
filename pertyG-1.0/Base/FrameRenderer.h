#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <memory>
#include <chrono>
#include <condition_variable>
#include <mutex>
namespace pertyG
{
    class IFrameEventListener
    {
    public:
        virtual void onFrameInitialized()
        {
        }
        virtual void onFrameProcessed()
        {
        }
        virtual void onFrameRendered()
        {
        }
    };
    class FrameRenderer
    {
    private:
        static std::unique_ptr<FrameRenderer> mInstance;
        std::mutex mEventListenerMutex;
        std::vector<IFrameEventListener *> mEventListenerList;
        double mFps;
        std::thread mTimerThread;
        bool mTimerRunning;
        void startTimer()
        {
            mTimerRunning = true;
            mTimerThread = std::thread(&FrameRenderer::framePolling, this);
        }
        void framePolling()
        {
            while (mTimerRunning)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0 / mFps)));
                {
                    std::lock_guard<std::mutex> lock(mEventListenerMutex);
                    for (auto listener : mEventListenerList)
                    {
                        listener->onFrameInitialized();
                    }
                    for (auto listener : mEventListenerList)
                    {
                        listener->onFrameProcessed();
                    }
                    for (auto listener : mEventListenerList)
                    {
                        listener->onFrameRendered();
                    }
                }
            }
        }
    public:
        FrameRenderer() : FrameRenderer(60.0)
        {
        }
        FrameRenderer(double fps)
        {
            setFps(fps);
            startTimer();
        }

        void stopTimer()
        {
            mTimerRunning = false;
            if (mTimerThread.joinable())
            {
                mTimerThread.join();
            }
        }

        static FrameRenderer *instance()
        {
            if (!mInstance)
            {
                mInstance = std::make_unique<FrameRenderer>();
            }
            return mInstance.get();
        }
        void setFps(double fps)
        {
            mFps = fps;
        }
        void addListener(IFrameEventListener *listener)
        {
            if (listener == nullptr)
            {
                std::cout << "don't regist null pointer" << std::endl;
                return;
            }
            mEventListenerList.push_back(listener);
        }
        void removeListener(IFrameEventListener *listener)
        {
            for (int i = 0; i < mEventListenerList.size(); i++)
            {
                if (mEventListenerList[i] == listener)
                {
                    mEventListenerList.erase(mEventListenerList.begin() + i);
                }
            }
        }
    };
}