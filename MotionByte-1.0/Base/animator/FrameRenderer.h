#pragma once
#include <vector>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>
#include "IFrameEventListener.h"
namespace MotionByte
{
    class IFrameEventListener;

    class FrameRenderer
    {
    private:
        static std::unique_ptr<FrameRenderer> mInstance;
        std::mutex mEventListenerMutex;
        std::vector<IFrameEventListener *> mEventListenerList;
        std::mutex mWaitlistMutex;
        std::vector<IFrameEventListener *> mSubscriberWaitList;
        std::vector<IFrameEventListener *> mUnsubscriberWaitList;
        double mFps;
        bool isNextTick = false;

        std::thread mPollingThread;
        std::thread mTimerThread;
        std::condition_variable poller;
        std::mutex pollerMutex;
        std::atomic<bool> mTimerRunning;
        void timerCall();
        void framePolling();
        //private constructor for singleton
        void processWaitList();
        void notifyListener();
    public:
        FrameRenderer();
        FrameRenderer(double fps);
        ~FrameRenderer();
        void startTimer();
        void stopTimer();

        static FrameRenderer *instance();
        void setFps(double fps);
        void addListener(IFrameEventListener *listener);
        void removeListener(IFrameEventListener *listener);
    };
}
