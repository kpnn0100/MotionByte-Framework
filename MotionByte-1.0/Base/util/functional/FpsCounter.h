#pragma once
#include <queue>
#include <chrono>
#include <string>
using namespace std;
#define MAX_FRAME_COUNT 100
namespace MotionByte
{
    class FpsCounter
    {
        private:
            queue<chrono::time_point<chrono::high_resolution_clock>> frameTimes;
            double fps;
            double frameTime;
            chrono::time_point<chrono::high_resolution_clock> lastFrameTime;
            int maxFrameCount;
        public:
            FpsCounter();
            void tick();
            double getFps();
            string getFpsString();
    };
}