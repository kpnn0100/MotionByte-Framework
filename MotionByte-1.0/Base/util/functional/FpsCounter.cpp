#include "FpsCounter.h"
namespace MotionByte
{
    FpsCounter::FpsCounter() : maxFrameCount(MAX_FRAME_COUNT), fps(0.0), frameTime(0.0)
    {
    }

    void FpsCounter::tick()
    {
        auto now = chrono::high_resolution_clock::now();
        frameTimes.push(now);
        if (frameTimes.size() > maxFrameCount)
        {
            frameTimes.pop();
        }
        if (frameTimes.size() > 1)
        {
            frameTime = chrono::duration_cast<chrono::milliseconds>(now - frameTimes.front()).count() / 1000.0;
            fps = (frameTimes.size() - 1) / frameTime;
        }
        lastFrameTime = now;
    }

    double FpsCounter::getFps()
    {
        return fps;
    }

    string FpsCounter::getFpsString()
    {
        return to_string(fps);
    }
}