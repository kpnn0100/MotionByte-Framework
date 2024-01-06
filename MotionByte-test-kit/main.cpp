#include "../MotionByte-1.0/Base/Window/Window.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace MotionByte;
class TempObject : public Segment
{
    Color myColor;
    double value;
    Property percent;
    void onMouseDragged(Point point) override
    {
        value = 1.0 - point.getY() / getLocalBound().getHeight();
        value /= 2.0;
        if (value < 0.0) value = 0.0;
        if (value > 1.0) value = 1.0;
        percent.setValue(value);
    }
public:
    TempObject()
    {
        mSegmentTypeName = "TempObject";
        value = 0.0;
        percent.setInterpolator(InterpolatorFactory::createSmooth(4.0, 4.0));
        percent.setValue(value);
        myColor.getAlpha().setPropertyName("alpha");
        //myColor.getAlpha().bind([this]
        //    {
        //        return percent.getValue();
        //    });
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
        frame.fillColor(Color(0, 255, 0, 255));
        double finalDegree = 240 + percent.getValue() * (-60 - 240);
        //frame.drawArc(myColor, this->getLocalBound(),4.0, 240, finalDegree, Frame::ClockWise);
        frame.fillRoundedRectangle(myColor, this->getLocalBound(), percent.getValue()*100.0);
        //frame.drawAnnularArc(myColor, this->getLocalBound().getLocalCenter(), 4.0,20.0, 240, finalDegree, Frame::ClockWise);
    }
};
std::shared_ptr<TempObject> temp = std::make_shared<TempObject>();
bool running = true;
void submain(Window * window) {
    while (running)
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
    temp->setBound(Rectangle(Point(50.5, 60.5), 200, 200));
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
    running = false;
    submainThread.join();
    return 0;
}
