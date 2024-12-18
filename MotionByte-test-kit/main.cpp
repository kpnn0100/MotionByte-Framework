#include "MotionByteHeader.h"
#include "Floater.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace MotionByte;
std::shared_ptr<VisualObject> temp = std::make_shared<VisualObject>();
std::shared_ptr<Label> label = std::make_shared<Label>("Hello World");
std::shared_ptr<CheckBox> checkBox = std::make_shared<CheckBox>();
std::shared_ptr<Slider> slider = std::make_shared<Slider>(0,200.0,50);
bool running = true;
int main() {
    
    GraphicManager::setAntiAliasing(GraphicManager::Level::High);
    Window window(500, 600);
    checkBox->getBound().setPosition(Point(200, 100));
    checkBox->getObjectProperties().setInterpolatorForAll(InterpolatorFactory::createSmooth(1000, 1000));
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
    slider->setStep(0);
    slider->setOnValueChangedCallback([](double value)
        {
            // checkBox->setBound(checkBox->getBound().withSizeKeepCenter(value, value));
        });
    window.addSegment(slider);
    // window.addSegment(temp);
    label->setBound(Rectangle(Point(0, 0), 200, 200));
    label->setTextSize(200);
    label->getTextSize().setInterpolator(InterpolatorFactory::createSmooth(1000, 1000));
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
