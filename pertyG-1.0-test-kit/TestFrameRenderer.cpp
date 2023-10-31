#include "../pertyG-1.0/Base/FrameRenderer.h"
#include "../pertyG-1.0/Functional/Timer.h"

using namespace pertyG;
class MyFrameListener : public pertyG::IFrameEventListener
{

public:
    int a=0;
    Timer timer;
    MyFrameListener()
    {
        timer.start();
    }
    virtual void onFrameRendered() override
    {
        timer.lap();
        std::cout << "Frame rendered " << a << std::endl;
        a++;
    }
};

int main()
{
    // Create an instance of the FrameRenderer class
    pertyG::FrameRenderer* renderer = pertyG::FrameRenderer::instance();
    double fps = 60.0;
    int duration = 5;
    // Create a frame listener and add it to the renderer
    MyFrameListener frameListener;
    renderer->addListener(&frameListener);

    // Set the desired frame rate (e.g., 60 FPS)
    
    renderer->setFps(fps);
    Timer timer;
    timer.start();
    // Run the renderer for a few seconds
    std::this_thread::sleep_for(std::chrono::seconds(duration));

    // Remove the frame listener
    renderer->removeListener(&frameListener);

    // Stop the renderer
    renderer->stopTimer();
    timer.log();
    std::cout<<"expected frame rendered: "<<(double)duration * fps << std::endl;
    std::cout<<"frame rendered: "<< frameListener.a << std::endl;
    return 0;
}
