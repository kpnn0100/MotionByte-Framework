#include "../pertyG-1.0/Base/Window/Window.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace pertyG;

void submain(Window * window) {
    while (true)
    {
        int num1, num2;
        std::cout << "Enter two integers: ";
        std::cin >> num1 >> num2;
        window->setSize(num1, num2);
    }
}

int main() {
    Window window(500, 600);
    std::thread submainThread(submain, &window);
    window.show();

    
    // Create a thread to run submain


    // Wait for the submainThread to finish
    

    window.waitToClose();
    submainThread.join();
    return 0;
}
