#include "MotionByteHeader.h"
#include "Floater.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace MotionByte;
std::shared_ptr<VisualObject> temp = std::make_shared<VisualObject>();
std::shared_ptr<Label> label;
std::shared_ptr<CheckBox> checkBox = std::make_shared<CheckBox>();
std::shared_ptr<Slider> slider = std::make_shared<Slider>(0,200.0,50);
std::shared_ptr<TextBox> textBox;
bool running = true;
int main() {
    
    GraphicManager::setAntiAliasing(GraphicManager::Level::High);
    Window window(500, 600);
    textBox = std::make_shared<TextBox>();
    window.addSegment(textBox);
    textBox->setTopLeftPosition(Point(50, 60));


    
    window.show();
    window.waitToClose();
    running = false;
    return 0;
}
