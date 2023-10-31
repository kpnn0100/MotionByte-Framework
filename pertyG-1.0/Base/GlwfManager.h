#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
namespace pertyG
{
class GlwfManager
{
public:
    GlwfManager()
    {
        if (!glfwInit()) {
            
        }
        std::cout<<"GLFW done init"<<std::endl;
    }
    ~GlwfManager()
    {
        std::cout<<"Program exit"<<std::endl;
        glfwTerminate();
    }
};
GlwfManager instance;
}