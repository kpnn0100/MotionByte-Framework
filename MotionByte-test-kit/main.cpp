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
    void onScroll(Point point, double xValue, double yValue) override
    {
        value += yValue*10.0 / getLocalBound().getHeight();
        if (value < 0.0) value = 0.0;
        if (value > 1.0) value = 1.0;
        percent.setValue(value);
    }
    void onMouseDragged(Point point) override
    {

    }
    void onClicked(Point point) override
    {
        percent = 1.0 - std::abs(percent);
    }
public:
    TempObject()
    {
        mSegmentTypeName = "TempObject";
        value = 0.0;
        percent.setInterpolator(InterpolatorFactory::createSmooth(8.0, 8.0));
        percent.setValue(value);
        myColor.getAlpha().setPropertyName("alpha");
        setIsLimited(true);
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
        //frame.fillColor(Color(0, 255, 0, 255));
        //double finalDegree = 240 + percent.getValue() * (-60 - 240);
        //frame.drawArc(myColor, this->getLocalBound(),4.0, 240, finalDegree, Frame::ClockWise);
        //frame.fillRoundedRectangle(myColor, this->getLocalBound().withSizeKeepCenter(250.0,250.0), percent.getValue() * 100.0);
        frame.fillCircle(myColor, this->getLocalBound().withSizeKeepCenter(this->getLocalBound().getWidth() * percent*4.0, this->getLocalBound().getWidth() * percent * 4.0));
        //frame.fillRectangle(MotionByte::Color(100, 200, 125, 255),
        //    getLocalBound());
        //frame.drawAnnularArc(myColor, this->getLocalBound().getLocalCenter(), 4.0,20.0, 240, finalDegree, Frame::ClockWise);
        frame.drawText(myColor, "Hello", 20.0, getLocalBound(), Align::CenterMiddle);
        
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
    std::thread submainThread(submain, &window);
    window.show();

    
    // Create a thread to run submain


    // Wait for the submainThread to finish
    window.waitToClose();
    running = false;
    submainThread.join();
    return 0;
}
