#include "Frame.h"
namespace pertyG
{
    int Frame::windowWidth = 0;
    int Frame::windowHeight = 0;
    // Constructor that takes a std::shared_ptr<GLFWwindow>
    Frame::Frame(Rectangle* bound)
    {
        mBound = bound;
    }

    // Method to fill color in the buffer

    void Frame::onWindowSizeChanged(GLFWwindow* window, int width, int height)
    {
        windowWidth = width;
        windowHeight = height;
    }
    void Frame::fillColor(Color color)
	{
		glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
		glClear(GL_COLOR_BUFFER_BIT); 
	}
    void Frame::drawCircle(Color color, Rectangle bound, double stroke)
    {
        std::vector<float> vertices;
        const float radius = 0.5f;
        const int segments = 100;
        const float lineThickness = stroke;  // Adjust this value for the desired line thickness

        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * 3.14159265 * static_cast<float>(i) / static_cast<float>(segments);

            // Outer point
            float x_outer = (radius + lineThickness) * glm::cos(theta);
            float y_outer = (radius + lineThickness) * glm::sin(theta);
            vertices.push_back(x_outer);
            vertices.push_back(y_outer);

            // Inner point
            float x_inner = (radius - lineThickness) * glm::cos(theta);
            float y_inner = (radius - lineThickness) * glm::sin(theta);
            vertices.push_back(x_inner);
            vertices.push_back(y_inner);
        }

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 2);
    }
    void Frame::drawRectangle(Color color, Rectangle bound, double stroke)
    {
        std::vector<float> vertices;
        const float lineThickness = stroke/2;  // Adjust this value for the desired line thickness

        for (int i = 0; i < Rectangle::CornerCount; ++i) {
            Point currentCorner = bound.getCorner(i);
            Point nextCorner = bound.getCorner((i+1)% Rectangle::CornerCount);
            float x_outer = (currentCorner.getX().getValue() - lineThickness) / (double)windowWidth * 2.0 - 1.0;
            float y_outer = (currentCorner.getY().getValue() - lineThickness) / (double)windowHeight * 2.0 - 1.0;


            // Inner point
            float x_inner = (nextCorner.getX().getValue() + lineThickness) / (double)windowWidth * 2.0 - 1.0;
            float y_inner = (nextCorner.getY().getValue() + lineThickness) / (double)windowHeight * 2.0 - 1.0;
            vertices.push_back(x_outer);
            vertices.push_back(y_outer);
            vertices.push_back(x_inner);
            vertices.push_back(y_inner);
        }

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 2);
    }
    void Frame::drawSomething()
    {
        std::vector<float> vertices;
        const float radius = 0.5f;
        const int segments = 10;

        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * 3.14159265 * static_cast<float>(i) / static_cast<float>(segments);
            float x = radius * glm::cos(theta);
            float y = radius * glm::sin(theta);
            vertices.push_back(x);
            vertices.push_back(y);
        }

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);


        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 2);
    }
}