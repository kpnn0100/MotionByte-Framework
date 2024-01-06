#pragma once

namespace MotionByte
{
    class IFrameEventListener
    {
    public:
        IFrameEventListener();
        virtual ~IFrameEventListener();
        virtual void onFrameInitialized();
        virtual void onFrameProcessed();
        virtual void onFrameRendered();
    };
}
