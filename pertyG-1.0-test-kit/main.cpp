#include "../pertyG-1.0/Base/Window/Window.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace pertyG;
class TempObject : public Segment
{
    Color myColor;
public:
    TempObject()
    {
        mSegmentTypeName = "TempObject";
    }
    void setColor(double red, double green, double blue, double alpha)
    {
        myColor.setRed(red);
        myColor.setGreen(green);
        myColor.setBlue(blue);
        myColor.setAlpha(alpha);
    }
    Color& getColor()
    {
        return myColor;
    }
    void paint(Frame& frame) override
    {
        frame.drawRectangle(myColor, this->getLocalBound(),6.0f);
    }
};
std::shared_ptr<TempObject> temp = std::make_shared<TempObject>();
void submain(Window * window) {
    while (true)
    {
        int num1, num2, num3, num4;
        std::cout << "Enter 4 integers for color (R, G, B, A): " << std::endl;
        std::cin >> num1 >> num2 >> num3 >> num4;
        std::cout << std::endl;
        temp->setColor((double)num1 / 255.0, (double)num2 / 255.0, (double)num3 / 255.0, (double)num4 / 255.0);
    }
}

int main() {
    Window window(500, 600);

    window.addSegment(temp);
    temp->setBound(Rectangle(Point(50, 60), 50, 50));
    temp->setColor(1, 0.5, 0.25, 1);
    temp->getSegmentPropertyManager().setInterpolatorForAll(InterpolatorFactory::createSmooth(1000, 1000));
    window.setClickCallback([](Point point)
        {
            temp->setCenterPosition(point);
        });
    std::thread submainThread(submain, &window);
    window.show();

    
    // Create a thread to run submain


    // Wait for the submainThread to finish
    

    window.waitToClose();
    submainThread.join();
    return 0;
}
