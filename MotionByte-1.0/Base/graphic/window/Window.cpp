#include "Window.h"
void APIENTRY GLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data) {
	printf("%d: %s\n", id, msg);
}
namespace MotionByte
{
    Window::Window(int width, int height) : mPropertyManager(PropertyCount)
    {
        mTopParent = this;
        mParent = nullptr;
        mMainWindow = nullptr;
        create(width, height, "hello");
    }
    void Window::create(int width, int height, const char* title)
    {
        if (mMainWindow == nullptr)
        {
            mMainWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
            mBound = Rectangle(Point(0.0, 0.0), (double)width, (double)height);
        }
    }
    void Window::addTask(std::function<void()> task)
    {
        std::lock_guard<std::mutex> lock(mTaskListLocker);
        mTaskList.push(task);
    }
    
    void Window::show()
    {
        if (!mMainWindow)
        {
            debug("Nothing to show");
            return;
        }
        handleWindow();
    }
    void Window::handleWindow()
    {

        // Make the window's context current
        glfwMakeContextCurrent(mMainWindow);

        // Enable anti-aliasing (multisampling)

        glewExperimental = true;
        glewInit();
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLDebug, NULL);
        FontManager::instance();

        onWindowSizeChanged((double)mBound.getWidth(), (double)mBound.getHeight());
        mainFrame.setWindow(this);
        //mainFrame.fillColor(Color(0, 0, 0, 255));
        glfwSetWindowUserPointer(mMainWindow, this);
        glfwSetWindowSizeCallback(mMainWindow, [](GLFWwindow* window, int width, int height)
            {
                Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
                instance->onWindowSizeChanged(width, height);
            });
        GLFWmousebuttonfun callbackFunction = [](GLFWwindow* window, int button, int action, int mods) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            Window::MouseButton mouseButton = MouseButton::Left;
            Window::MouseAction mouseAction = MouseAction::Pressed;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                instance->pressAt(Point(xpos, ypos));
            }
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
                instance->releaseAt(Point(xpos, ypos));
            }
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouseButton = MouseButton::Left;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                mouseButton = MouseButton::Right;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                mouseButton = MouseButton::Middle;
                break;
            default:
                break;
            }
            switch (action)
            {
            case GLFW_PRESS:
                mouseAction = MouseAction::Pressed;
                break;
            case GLFW_RELEASE:
                mouseAction = MouseAction::Released;
                break;
            default:
                break;
            }
            instance->mouseAction(Point(xpos, ypos),mouseButton,mouseAction);
        };

        glfwSetCursorPosCallback(mMainWindow,
            [](GLFWwindow* window, double xpos, double ypos)
            {
                Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
                instance->mouseMove(Point(xpos, ypos));
            }
        );
        glfwSetScrollCallback(mMainWindow, [](GLFWwindow* window,double xPos, double yPos)
            {
                Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
                double mouseX, mouseY;
                glfwGetCursorPos(window, &mouseX, &mouseY);
                instance->scrollAt(Point(mouseX, mouseY),xPos,yPos);
            });
        glfwSetMouseButtonCallback(mMainWindow, callbackFunction);
        GraphicManager::getInstance();
        
        // Enter the rendering loop in a separate thread
        while (!glfwWindowShouldClose(mMainWindow))
        {
            //temp
            
            if (mIsFrameProcessed == false)
            {
                glClear(GL_COLOR_BUFFER_BIT);
                PausableTimer::getInstance().pause();
                triggerPaint();
                PausableTimer::getInstance().resume();
                mIsFrameProcessed = true;
                glfwSwapBuffers(mMainWindow);
            }

            // Swap front and back buffers
            

            // Poll for and process events
            glfwPollEvents();
        }
    }
    void Window::setSize(int width, int height)
    {
        mBound.setSize(width, height);
    }
    void Window::onFrameInitialized()
    {
        int a = 2;
    }
    void Window::onFrameProcessed()
    {
        if (mMainWindow)
        {
            mIsFrameProcessed = false;  
        }
    }
    void Window::onFrameRendered()
    {
    }
    void Window::waitToClose()
    {
        if (renderThread.joinable())
        {
            renderThread.join();
        }
    }
    void Window::close()
    {
        if (mMainWindow)
        {
            glfwSetWindowShouldClose(mMainWindow, GL_TRUE);
        }
        if (renderThread.joinable())
        {
            renderThread.join();
        }
    }
    void Window::paint(Frame& frame)
    {
        frame.fillColor(Color(0, 0, 0,255));
        //mainFrame.drawCircle(Color(50, 50, 50), mBound.withSizeKeepCenter(200,300), 0.01);
    }
    PropertyManager& Window::getPropertyManager()
    {
        return mPropertyManager;
    }
    void Window::onWindowSizeChanged(int width, int height)
    {
        ShapeManager::instance().onWindowSizeChanged(width, height);
        FontManager::instance().onWindowSizeChanged(width, height);
        Frame::onWindowSizeChanged(mMainWindow, width, height);
        mBound.setSize(width, height);
    }
    Window::~Window()
    {
        if (mMainWindow)
        {
            glfwDestroyWindow(mMainWindow);
        }
    }
}