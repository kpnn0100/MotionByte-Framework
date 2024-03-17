#include "FrameRenderer.h"
#include <thread>
#include <iostream>
#include "debugging_tool/DEBUG.h"
namespace MotionByte
{
    std::unique_ptr<FrameRenderer> FrameRenderer::mInstance = nullptr;

    FrameRenderer::FrameRenderer() : FrameRenderer(60.0) {}

    FrameRenderer::FrameRenderer(double fps)
    {
#ifndef DEBUG
        setFps(fps);
#else
        setFps(fps/6.0);
#endif

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
                processWaitList();
                notifyListener();
                isNextTick = false;
            }
        }
    }

    void FrameRenderer::processWaitList()
    {
        std::lock_guard<std::mutex> lock(mEventListenerMutex);
        std::lock_guard<std::mutex> lock2(mWaitlistMutex);
        //process unsub list
        for (int i = 0; i < mUnsubscriberWaitList.size(); i++)
        {
            for (int j = 0; j < mEventListenerList.size(); j++)
            {
                if (mUnsubscriberWaitList[i] == mEventListenerList[j])
                {
                    mEventListenerList.erase(mEventListenerList.begin() + j);
                    mUnsubscriberWaitList.erase(mUnsubscriberWaitList.begin() + i);
                    i -=1;
                    break;
                }
            }
        }
        //process sub list
        for (int i = 0; i < mSubscriberWaitList.size(); i++)
        {
            mEventListenerList.push_back(mSubscriberWaitList[i]);
        }
        mSubscriberWaitList.clear();
    }

    void FrameRenderer::notifyListener()
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
        std::lock_guard<std::mutex> lock(mWaitlistMutex);
        mSubscriberWaitList.push_back(listener);
    }

    void FrameRenderer::removeListener(IFrameEventListener *listener)
    {
        std::lock_guard<std::mutex> lock(mWaitlistMutex);
        for (int i = 0; i < mSubscriberWaitList.size(); i++)
        {
            if (mSubscriberWaitList[i] == listener)
            {
                mSubscriberWaitList.erase(mSubscriberWaitList.begin() + i);
                return;
            }
        }
        mUnsubscriberWaitList.push_back(listener);
    }
}
