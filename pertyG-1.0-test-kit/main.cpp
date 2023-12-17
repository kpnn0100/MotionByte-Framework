#include "../pertyG-1.0/Base/Window/Window.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace pertyG;

void submain(Window * window) {
    while (true)
    {
        int num1, num2;
        std::cout << "Enter two integers: " << std::endl;
        std::cin >> num1 >> num2;
        std::cout << std::endl;
        window->setSize(num1, num2);
    }
}

int main() {
    Window window(500, 600);
    window.getPropertyManager().getProperty(Window::WindowWidth).setInterpolator(InterpolatorFactory::createLinear(2.0));
    window.getPropertyManager().getProperty(Window::WindowHeight).setInterpolator(InterpolatorFactory::createLinear(2.0));
    std::thread submainThread(submain, &window);
    window.show();

    
    // Create a thread to run submain


    // Wait for the submainThread to finish
    

    window.waitToClose();
    submainThread.join();
    return 0;
}
