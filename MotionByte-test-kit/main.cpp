#include "MotionByteHeader.h"
#include "Floater.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace MotionByte;
std::shared_ptr<VisualObject> temp = std::make_shared<VisualObject>();
std::shared_ptr<Label> label = std::make_shared<Label>("Hello World");
std::shared_ptr<CheckBox> checkBox = std::make_shared<CheckBox>();
std::shared_ptr<Slider> slider = std::make_shared<Slider>(0,100,50);
bool running = true;
int main() {
    GraphicManager::setAntiAliasing(GraphicManager::Level::High);
    Window window(500, 600);
    checkBox->getBound().setPosition(Point(200, 100));
    checkBox->setStateCallback([](CheckBox::ButtonState state)
        {
            if (state == CheckBox::ButtonState::Pressed)
            {
                slider->setDragType(Slider::DragType::Drag);
            }
            else
            {
                slider->setDragType(Slider::DragType::Click);
            }
        });
    window.addSegment(checkBox);
    slider->setBound(Rectangle(Point(200, 200), 600, 20));
    // slider->setStep(10);
    window.addSegment(slider);
    // window.addSegment(temp);
    label->setBound(Rectangle(Point(0, 0), 200, 200));
    label->setTextSize(200);
    label->getTextSize().setInterpolator(InterpolatorFactory::createSmooth(1000, 1000));
    temp->addSegment(label);
    temp->setBound(Rectangle(Point(50.5, 60.5), 200, 200));
    temp->setBorderThickness(50.0);
    temp->setRadius(10.0f);
    temp->setBackgroundColor(Color(0.5, 0.2, 0.1, 0.9));
    temp->setBorderColor(Color(0.9, 0.8, 0.5, 0.9));
    temp->getObjectProperties().setInterpolatorForAll(InterpolatorFactory::createSmooth(1000, 1000));
    temp->setOpacity(0.5);
    temp->setScrollCallback([](Point point, double x, double y)
        {
            double currentRadius = temp->getObjectProperties().getValue(temp->Radius);
            temp->setRadius(currentRadius + y*5);
            double currentTextSize = label->getTextSize();
            label->setTextSize(currentTextSize + y*5);
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
