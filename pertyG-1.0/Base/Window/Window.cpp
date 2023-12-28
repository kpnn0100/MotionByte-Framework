#include "Window.h"
const char* vertexShaderSource = R"(
    #version 330 core

    // Input vertex data, different for all executions of this shader.
    layout(location = 0) in vec2 vertexPosition_modelspace;
    layout(location = 1) in vec4 vertexColor;

    // Output data ; will be interpolated for each fragment.
    out vec4 fragmentColor;
    // Values that stay constant for the whole mesh.

    void main(){	

	     gl_Position = vec4(vertexPosition_modelspace, 0.0, 1.0);

	    // The color of each vertex will be interpolated
	    // to produce the color of each fragment
	    fragmentColor = vertexColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core

    // Interpolated values from the vertex shaders
    in vec4 fragmentColor;

    // Ouput data
    out vec4 color;

    void main(){

    // Output color = color specified in the vertex shader, 
    // interpolated between all 3 surrounding vertices
    color = fragmentColor;

    }
)";
namespace pertyG
{

    Window::Window(int width, int height) : mPropertyManager(PropertyManager(PropertyCount))
    {
        mTopParent = this;
        mMainWindow = nullptr;
        create(width, height, "hello");
        mPropertyManager.initValue(WindowWidth, width);
        mPropertyManager.initValue(WindowHeight, height);
    }
    GLuint& Window::getVertexBuffer()
    {
        return vertexBuffer;
    }
    GLuint& Window::getColorBuffer()
    {
        return colorBuffer;
    }
    void Window::addTask(std::function<void()> task)
    {
        std::lock_guard<std::mutex> lock(mTaskListLocker);
        mTaskList.push(task);
    }
    void Window::create(int width, int height, const char* title)
    {
        if (mMainWindow == nullptr)
        {
            if (!glfwInit())
            {
                return;
            }

            // Set GLFW window hints (optional)
            // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_SAMPLES, 4);
            mMainWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
            Frame::onWindowSizeChanged(mMainWindow,width,height);
            glfwSetWindowSizeCallback(mMainWindow, Frame::onWindowSizeChanged);
            mBound = Rectangle(Point(0.0, 0.0), (double)width, (double)height);
            if (!mMainWindow)
            {
                // Window creation failed
                // You can add error handling or logging here
                return;
            }


        }
    }
    void Window::show()
    {
        if (mMainWindow)
        {
            handleWindow();
        }
    }
    void Window::handleWindow()
    {
        // Make the window's context current
        glfwMakeContextCurrent(mMainWindow);
        // Enable anti-aliasing (multisampling)
        glEnable(GL_MULTISAMPLE);
        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            getchar();
            glfwTerminate();
            return;
        }

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glUseProgram(shaderProgram);

        GLuint vertexArrayID;
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);


        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);


        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mMainFrame->fillColor(Color(0, 0, 0, 255));

        // glfwSetMouseButtonCallback(mMainWindow, [](GLFWwindow* window, int button, int action, int mods) {
        // if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        //     double xpos, ypos;
        //     glfwGetCursorPos(window, &xpos, &ypos);
        //     std::cout << "Mouse clicked at position: (" << xpos << ", " << ypos << ")" << std::endl;
        // }
        // });
        // Enter the rendering loop in a separate thread
        while (!glfwWindowShouldClose(mMainWindow))
        {
            //temp

            // Your rendering code here
            {
                std::lock_guard<std::mutex> lock(mTaskListLocker);
                for (; !mTaskList.empty(); mTaskList.pop())
                {
                    mTaskList.front()();
                }
            }


            // Swap front and back buffers
            glfwSwapBuffers(mMainWindow);

            // Poll for and process events
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteProgram(shaderProgram);
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
            addTask([this]
                {
                    triggerPaint();
                });
            
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
    Window::~Window()
    {
        if (mMainWindow)
        {
            glfwDestroyWindow(mMainWindow);
        }
    }
}