#pragma once
#include <vector>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>
#include "IFrameEventListener.h"
namespace pertyG
{
    class IFrameEventListener;

    class FrameRenderer
    {
    private:
        static std::unique_ptr<FrameRenderer> mInstance;
        std::mutex mEventListenerMutex;
        std::vector<IFrameEventListener *> mEventListenerList;
        double mFps;
        bool isNextTick = false;

        std::thread mPollingThread;
        std::thread mTimerThread;
        std::condition_variable poller;
        std::mutex pollerMutex;
        std::atomic<bool> mTimerRunning;

        void startTimer();
        void timerCall();
        void framePolling();

    public:
        FrameRenderer();
        FrameRenderer(double fps);
        ~FrameRenderer();
        void stopTimer();

        static FrameRenderer *instance();
        void setFps(double fps);
        void addListener(IFrameEventListener *listener);
        void removeListener(IFrameEventListener *listener);
    };
}
