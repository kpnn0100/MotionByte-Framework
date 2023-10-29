#pragma once
#include <GLFW/glfw3.h>
#include <vector>
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
    static std::vector<IFrameEventListener*> mEventListenerList;
};
}