#pragma once
#include <chrono>
#include <iostream>
#include <mutex>

namespace MotionByte
{
    class PausableTimer
    {
    public:
        static PausableTimer& getInstance()
        {
            static PausableTimer instance;
            return instance;
        }

        // Return the current time, considering pause/resume
        std::chrono::time_point<std::chrono::high_resolution_clock> now()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (isPaused)
            {
                return pauseTime;
            }
            else
            {
                return std::chrono::high_resolution_clock::now();
            }
        }

        // Pause the timer
        void pause()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!isPaused)
            {
                isPaused = true;
                pauseTime = std::chrono::high_resolution_clock::now();
            }
        }

        // Resume the timer
        void resume()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (isPaused)
            {
                isPaused = false;
                auto pausedDuration = std::chrono::high_resolution_clock::now() - pauseTime;
                pauseTime = std::chrono::high_resolution_clock::now() - pausedDuration;
            }
        }

    private:
        PausableTimer() : isPaused(false) {}

        PausableTimer(const PausableTimer&) = delete;
        PausableTimer& operator=(const PausableTimer&) = delete;

        bool isPaused;
        std::chrono::time_point<std::chrono::high_resolution_clock> pauseTime;
        std::mutex mutex_;
    };
}
