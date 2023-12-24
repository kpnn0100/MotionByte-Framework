#include "FrameRenderer.h"
#include <thread>
#include <iostream>

namespace pertyG
{
    std::unique_ptr<FrameRenderer> FrameRenderer::mInstance = nullptr;

    FrameRenderer::FrameRenderer() : FrameRenderer(60.0) {}

    FrameRenderer::FrameRenderer(double fps)
    {
        setFps(fps);
        startTimer();
    }

    FrameRenderer::~FrameRenderer()
    {
        stopTimer();
    }

    void FrameRenderer::startTimer()
    {
        mTimerRunning = true;
        isNextTick = true;
        mTimerThread = std::thread(&FrameRenderer::timerCall, this);
        mPollingThread = std::thread(&FrameRenderer::framePolling, this);
    }

    void FrameRenderer::timerCall()
    {
        double targetMicroseconds = 1000000.0 / mFps;
        std::chrono::high_resolution_clock::time_point nextTickTime = std::chrono::high_resolution_clock::now();

        while (mTimerRunning)
        {
            nextTickTime += std::chrono::microseconds(static_cast<int>(targetMicroseconds));
            std::this_thread::sleep_until(nextTickTime);
            isNextTick = true;
            poller.notify_all();
        }
    }

    void FrameRenderer::framePolling()
    {
        while (mTimerRunning)
        {
            std::unique_lock<std::mutex> lock(pollerMutex);
            poller.wait(lock, [this] { return isNextTick; });
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
                isNextTick = false;
            }
        }
    }

    void FrameRenderer::stopTimer()
    {
        mTimerRunning = false;
        if (mTimerThread.joinable())
        {
            mTimerThread.join();
        }
        if (mPollingThread.joinable())
        {
            mPollingThread.join();
        }
    }

    FrameRenderer *FrameRenderer::instance()
    {
        if (!mInstance)
        {
            mInstance = std::make_unique<FrameRenderer>();
        }
        return mInstance.get();
    }

    void FrameRenderer::setFps(double fps)
    {
        mFps = fps;
    }

    void FrameRenderer::addListener(IFrameEventListener *listener)
    {
        if (listener == nullptr)
        {
            std::cout << "Don't register null pointer" << std::endl;
            return;
        }
        //std::lock_guard<std::mutex> lock(mEventListenerMutex);
        mEventListenerList.push_back(listener);
    }

    void FrameRenderer::removeListener(IFrameEventListener *listener)
    {
        std::lock_guard<std::mutex> lock(mEventListenerMutex);
        for (int i = 0; i < mEventListenerList.size(); i++)
        {
            if (mEventListenerList[i] == listener)
            {
                mEventListenerList.erase(mEventListenerList.begin() + i);
            }
        }
    }
}
