#include "FrameRenderer.h"
#include "IFrameEventListener.h"
namespace pertyG
{
    IFrameEventListener::IFrameEventListener()
    {
        FrameRenderer::instance()->addListener(this);
    }
    IFrameEventListener::~IFrameEventListener()
    {
        FrameRenderer::instance()->removeListener(this);
    }
    void IFrameEventListener::onFrameInitialized()
    {
        
    }
    void IFrameEventListener::onFrameProcessed()
    {
    }
    void IFrameEventListener::onFrameRendered()
    {
    }
}