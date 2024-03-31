#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <memory>
#include <iostream>
namespace MotionByte
{
class GraphicManager
{
private:
    static std::shared_ptr<GraphicManager> instance;
public:
    GraphicManager();
    ~GraphicManager();
    enum Level
    {
        Low,
        Medium,
        High
    };
    static GraphicManager& getInstance();
    static void setAntiAliasing(Level level);
};

}