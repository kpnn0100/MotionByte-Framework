#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "GlwfManager.h"

#include <atomic>
#include <queue>
#include <functional>
#include <thread>


#include <vector>

#include "../FrameRenderer/FrameRenderer.h"
#include "../Property/PropertyHeader.h"
#include "../Property/PropertyManager.h"
#include "../Graphic/Frame.h"
#include "../Segment/Segment.h"
namespace MotionByte
{
    class Window : public IFrameEventListener, public Segment
    {
    private:
        std::atomic<GLFWwindow*> mMainWindow;
        PropertyManager mPropertyManager;
        std::mutex mTaskListLocker;
        std::queue<std::function<void()>> mTaskList;
        Property size[2];
        bool mNeedUpdate = true;
        std::thread renderThread;
        GLuint vertexBuffer;
        GLuint colorBuffer;
    public:
        enum PropertyList
        {
            WindowWidth,
            WindowHeight,
            PropertyCount
        };
        Window(int width, int height);
        GLuint& getVertexBuffer();
        GLuint& getColorBuffer();
        void addTask(std::function<void()> task);
        void handleWindow();
        void create(int width, int height, const char* title);

        void show();

        void setSize(int width, int height);
        void onFrameInitialized() override;
        void onFrameProcessed() override;
        void onFrameRendered() override;
        void waitToClose();
        void close();
        virtual void paint(Frame &frame) override;
        PropertyManager& getPropertyManager();
        ~Window();
    };
}
