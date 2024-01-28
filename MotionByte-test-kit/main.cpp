#include "../MotionByte-1.0/Base/Window/Window.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace MotionByte;
std::shared_ptr<VisualObject> temp = std::make_shared<VisualObject>();
bool running = true;

int main() {
    Window window(500, 600);

    window.addSegment(temp);
    temp->setBound(Rectangle(Point(50.5, 60.5), 200, 200));
    temp->setBorderThickness(5.0f);
    temp->setRadius(10.0f);
    temp->setBackgroundColor(Color(0.5, 0.2, 0.1, 0.9));
    temp->setBorderColor(Color(0.9, 0.8, 0.5, 0.9));
    temp->getObjectProperties().setInterpolatorForAll(InterpolatorFactory::createSmooth(1000, 1000));
    temp->setScrollCallback([](Point point, double x, double y)
        {
            double currentRadius = temp->getObjectProperties().getValue(temp->Radius);
            temp->setRadius(currentRadius + y*20);
        });
    window.setClickCallback([](Point point)
        {
            temp->setCenterPosition(point);
        });
    window.show();

    
    // Create a thread to run submain


    // Wait for the submainThread to finish
    window.waitToClose();
    running = false;
    return 0;
}
