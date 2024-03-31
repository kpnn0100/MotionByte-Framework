/*
    MotionByte Framework
    Created by: kpnn0100 / Doan Le Nhat Nam
    Version: 1.0
*/
#include <iostream>
#include "GraphicManager.h"

namespace MotionByte {
    std::shared_ptr<GraphicManager> GraphicManager::instance = std::make_shared<GraphicManager>();
    GraphicManager::GraphicManager()
    {
        glfwInit();
        setAntiAliasing(Level::High);
    }
    GraphicManager& GraphicManager::getInstance()
    {
        return *instance;
    }
    GraphicManager::~GraphicManager()
    {
    }

    void GraphicManager::setAntiAliasing(Level level)
    {
        getInstance();
        switch(level)
        {
        case Level::Low:
            glfwWindowHint(GLFW_SAMPLES, 4);
            break;
        case Level::Medium:
            glfwWindowHint(GLFW_SAMPLES, 16);
            break;
        case Level::High:
            glfwWindowHint(GLFW_SAMPLES, 128);
            break;
        }
    }

}
