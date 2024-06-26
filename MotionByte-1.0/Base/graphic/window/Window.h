#pragma once
/*
    MotionByte Framework
    Created by: kpnn0100 / Doan Le Nhat Nam
    Version: 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include "GraphicManager.h"

#include <atomic>
#include <queue>
#include <functional>
#include <thread>
#include <vector>

#include "util/functional/FpsCounter.h"
#include "util/property/PropertyHeader.h"
#include "graphic/drawer/Frame.h"
#include "component/base-component/VisualObject.h"
#include "component/display-component/Label.h"
namespace MotionByte {
    /**
     * @brief The Window class represents a graphical window using GLFW for rendering.
     *        It inherits from IFrameEventListener and Segment, providing functionalities for
     *        handling window events and rendering graphics.
     */
    class Window : public Segment {
    private:
        std::shared_ptr<Label> mFps = std::make_shared<Label>("FPS: 0");
        std::atomic<bool> mIsFrameProcessed;
        std::atomic<GLFWwindow*> mMainWindow;     ///< Atomic pointer to the GLFW window.
        PropertyManager mPropertyManager;         ///< Manages properties associated with the window.
        std::mutex mTaskListLocker;               ///< Mutex for synchronizing access to the task list.
        std::queue<std::function<void()>> mTaskList; ///< Queue of tasks to be executed.
        Property size[2];                         ///< Width and height properties of the window.
        bool mNeedUpdate = true;                  ///< Flag indicating whether the window needs an update.
        std::thread renderThread;                 ///< Thread for rendering the window.
    public:
        /**
         * @brief Enum listing properties associated with the Window class.
         */
        enum PropertyList {
            WindowWidth,      ///< Width of the window.
            WindowHeight,     ///< Height of the window.
            PropertyCount     ///< Number of properties.
        };
        enum Program
        {
            Text,
            Shape,
            ProgramCount
        };
        /**
         * @brief Constructor for the Window class.
         * @param width Width of the window.
         * @param height Height of the window.
         */
        Window(int width, int height);

        /**
         * @brief Adds a task to the task list.
         * @param task Task to be added to the task list.
         */
        void addTask(std::function<void()> task);

        /**
         * @brief Handles the window creation and initializes GLFW.
         * @param width Width of the window.
         * @param height Height of the window.
         * @param title Title of the window.
         */
        void create(int width, int height, const char* title);

        /**
         * @brief Displays the window and enters the rendering loop.
         *        This function is the main entry point for the window to start rendering.
         */
        void show();

        /**
         * @brief Handles the rendering loop and GLFW events for the window.
         *        This function is responsible for continuously rendering the window and handling events.
         */
        void handleWindow();

        /**
         * @brief Sets the size of the window.
         * @param width New width of the window.
         * @param height New height of the window.
         */
        void setSize(int width, int height);

        /**
         * @brief Waits for the render thread to close.
         *        This function ensures that the render thread is properly closed before continuing.
         */
        void waitToClose();

        /**
         * @brief Closes the window.
         *        This function sets the GLFW window should close flag and waits for the render thread to finish.
         */
        void close();

        /**
         * @brief Paints the window with the provided frame.
         * @param frame The frame used for painting the window.
         */
        virtual void paint(Frame& frame) override;

        /**
         * @brief Getter for the property manager.
         * @return Reference to the PropertyManager associated with the window.
         */
        PropertyManager& getPropertyManager();
        /**
         * @brief Call when window size changed
         *  
         */
        void onWindowSizeChanged(int width, int height);
        /**
         * @brief Destructor for the Window class.
         *        Cleans up GLFW resources and destroys the window.
         */
        ~Window();
    };
}
