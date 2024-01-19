#include "Window.h"
const char* vertexShaderSource = R"(
    #version 460 core

    // Input vertex data, different for all executions of this shader.
    layout(location = 0) in vec2 vertexPosition_modelspace;
    layout(location = 1) in vec4 vertexColor;
	layout (location = 2) uniform mat4 projection;

    // Output data ; will be interpolated for each fragment.
    out vec4 fragmentColor;
    // Values that stay constant for the whole mesh.

    void main(){	
        gl_Position = projection * vec4(vertexPosition_modelspace, 0.0, 1.0);
        gl_Position.y = -gl_Position.y;
        //gl_Position = vec4(vertexPosition_modelspace, 0.0, 1.0);
	    // The color of each vertex will be interpolated
	    // to produce the color of each fragment
	    fragmentColor = vertexColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 460 core

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
const char* textShaderSource = R"(
	#version 460 core
	layout (location = 3) in vec4 vertex; // <vec2 pos, vec2 tex>
	layout (location = 4) uniform mat4 projection;

	out vec2 TexCoords;

	void main()
	{
		gl_Position = projection * vec4(vertex.x, vertex.y, 0.0, 1.0);
		TexCoords = vertex.zw;
	}  
)";

const char* textFragmentShaderSource = R"(
	#version 460 core
	in vec2 TexCoords;
	out vec4 color;

	layout (binding = 0) uniform sampler2D text;
	layout (location = 6) uniform vec4 textColor;

	void main()
	{    
		color = textColor * texture(text, TexCoords).r;
	} 
)";
void APIENTRY GLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data) {
	printf("%d: %s\n", id, msg);
}
GLuint CompileShaders(const char* vertex, const  char* fragment) {

    GLuint shader_programme = glCreateProgram();

    GLuint vs, tcs, tes, gs, fs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex, NULL);
    glCompileShader(vs);

    glAttachShader(shader_programme, vs);

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment, NULL);
    glCompileShader(fs);


    glAttachShader(shader_programme, fs);
    glLinkProgram(shader_programme);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return shader_programme;
}
namespace MotionByte
{
    void Window::updateUniform()
    {
        GLint projectionLocation = glGetUniformLocation(*currentProgram, "projection");
        glm::mat4 projection = glm::ortho(0.0f, (float)mBound.getWidth(), 0.0f, (float)mBound.getHeight());
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    }
    Window::Window(int width, int height) : mPropertyManager(PropertyCount)
    {
        mTopParent = this;
        mParent = nullptr;
        mMainWindow = nullptr;
        create(width, height, "hello");
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

        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            getchar();
            glfwTerminate();
            return;
        }
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEBUG_OUTPUT);

        glDebugMessageCallback(GLDebug, NULL);

        shaderProgram = CompileShaders(vertexShaderSource, fragmentShaderSource);

        changeProgram(Shape);


        textShaderProgram = CompileShaders(textShaderSource, textFragmentShaderSource);
        FontManager::instance();

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);


        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

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
            mIsFrameProcessed = false;
            //addTask([this]
            //    {
            //        //Free time during render to avoid glitch

            //    });
            
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
        //frame.fillColor(Color(0, 0, 0,255));
        //mainFrame.drawCircle(Color(50, 50, 50), mBound.withSizeKeepCenter(200,300), 0.01);
        frame.fillRectangle(MotionByte::Color(100, 200, 125, 255),
            Rectangle(Point(20, 60), 200.0, 500.0));
        std::string text("Hello OpenGL");
        GLfloat x = 50.0f;
        GLfloat y = 300.0f;
        GLfloat scale = 1.0f;
        frame.drawText(MotionByte::Color(100, 200, 125, 255), text, Point(x, y), scale);
    }
    PropertyManager& Window::getPropertyManager()
    {
        return mPropertyManager;
    }
    GLuint& Window::getMainShaderProgram()
    {
        return shaderProgram;
    }
    GLuint& Window::getTextShaderProgram()
    {
        return textShaderProgram;
    }
    void Window::changeProgram(Program program)
    {
        switch (program)
        {
            case Program::Shape:
            {
                currentProgram = &shaderProgram;
                glUseProgram(shaderProgram);
                break;
            }
            case Program::Text:
            {
                currentProgram = &textShaderProgram;
                glUseProgram(textShaderProgram);
                break;
            }
            default:
            {
                break;
            }
        }
        updateUniform();
    }
    void Window::onWindowSizeChanged(int width, int height)
    {
        Frame::onWindowSizeChanged(mMainWindow, width, height);
        mBound.setSize(width, height);
        updateUniform();
    }
    Window::~Window()
    {
        if (mMainWindow)
        {
            glfwDestroyWindow(mMainWindow);
        }
    }
}