/*
    MotionByte Framework
    Created by: kpnn0100 / Doan Le Nhat Nam
    Version: 1.0
*/

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

namespace MotionByte {

    /**
     * @brief The GlwfManager class is responsible for initializing and terminating the GLFW library.
     *        It ensures proper initialization of GLFW when an instance is created and terminates
     *        GLFW when the instance is destroyed.
     */
    class GlwfManager {
    public:
        /**
         * @brief Constructor for GlwfManager.
         *        Initializes the GLFW library.
         */
        GlwfManager();

        /**
         * @brief Destructor for GlwfManager.
         *        Cleans up GLFW resources and terminates the GLFW library.
         */
        ~GlwfManager();
    };
}
