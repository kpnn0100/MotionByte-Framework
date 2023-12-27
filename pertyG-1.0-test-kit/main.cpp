#include "../pertyG-1.0/Base/Window/Window.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace pertyG;
class TempObject : public Segment
{
public:
    void paint(Frame& frame) override
    {
        Rectangle drawer = mBound;
        drawer.setPosition(Point(0, 0));
        frame.fillRectangle(Color(120, 50, 50,255), mBound);
    }
};
std::shared_ptr<TempObject> temp = std::make_shared<TempObject>();
void submain(Window * window) {
    while (true)
    {
        int num1, num2;
        std::cout << "Enter two integers: " << std::endl;
        std::cin >> num1 >> num2;
        std::cout << std::endl;
        temp->getBound().setSize((double)num1, (double)num2);
    }
}

int main() {
    Window window(500, 600);

    window.addSegment(temp);
    temp->setBound(Rectangle(Point(50, 60), 100, 200));
    temp->getBound().getWidth().setInterpolator(InterpolatorFactory::createEaseInOut(2.0));
    temp->getBound().getHeight().setInterpolator(InterpolatorFactory::createEaseInOut(2.0));
    std::thread submainThread(submain, &window);
    window.show();

    
    // Create a thread to run submain


    // Wait for the submainThread to finish
    

    window.waitToClose();
    submainThread.join();
    return 0;
}
