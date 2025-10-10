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

    void GraphicManager::init()
    {
        glewExperimental = true;
        
        glewInit();
        setAntiAliasing(Level::High);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);  // Enable polygon smoothing
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Use the best quality for line smoothing
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);  // Use the best quality for polygon smoothing
        glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); // Better texture compression quality
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); // Better mipmap generation

        // Enable alpha blending for transparent textures
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEBUG_OUTPUT);
    }
}
