#include "Frame.h"
#include "../Window/Window.h"
#include <math.h>
namespace pertyG
{
    int Frame::windowWidth = 0;
    int Frame::windowHeight = 0;
    // Constructor that takes a std::shared_ptr<GLFWwindow>
    Frame::Frame(Rectangle* bound)
    {
        mBound = bound;
    }

    void Frame::setWindow(Window* window)
    {
        mWindow = window;
    }

    // Method to fill color in the buffer

    void Frame::onWindowSizeChanged(GLFWwindow* window, int width, int height)
    {
        windowWidth = width;
        windowHeight = height;
        glViewport(0, 0, width, height);
    }
    void Frame::fillColor(Color color)
	{
        std::vector<float> vertices;
        std::vector<float> colors;  // Add color information
        for (int i = 0; i <= Rectangle::CornerCount; ++i) {
            Point currentCorner = mBound->getCorner(i % Rectangle::CornerCount);
            float x_outer = (currentCorner.getX().getValue()) / (double)windowWidth * 2.0 - 1.0;
            float y_outer = (currentCorner.getY().getValue()) / (double)windowHeight * 2.0 - 1.0;
            y_outer = -y_outer;
            vertices.push_back(x_outer);
            vertices.push_back(y_outer);
            colors.push_back((double)color.getRed()); // R
            colors.push_back((double)color.getGreen()); // G
            colors.push_back((double)color.getBlue()); // B
            colors.push_back((double)color.getAlpha()); // Alpha
        }
        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getVertexBuffer());
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getColorBuffer());
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Draw
        glDrawArrays(GL_QUADS, 0, vertices.size() / 2);

        glBindVertexArray(0);
	}

    void Frame::drawRectangle(Color color, Rectangle bound, double stroke)
    {
        std::vector<float> vertices;
        const float lineThickness = stroke/2;  // Adjust this value for the desired line thickness
        std::vector<float> colors;  // Add color information
        for (int i = 0; i < Rectangle::CornerCount; ++i) 
        {
            Point currentCorner;
            Point nextCorner;
            if (i > 1)
            {
                nextCorner = bound.getCorner(i % Rectangle::CornerCount);
                currentCorner = bound.getCorner((i + 1) % Rectangle::CornerCount);
            }
            else
            {
                currentCorner = bound.getCorner(i % Rectangle::CornerCount);
                nextCorner = bound.getCorner((i + 1) % Rectangle::CornerCount);
            }

            float x[4];
            float y[4];
            double sign = i % 2 == 0 ? 1.0 : -1.0;
            x[0] = (mBound->getPosition().getX() + currentCorner.getX().getValue() - sign* lineThickness) / (double)windowWidth * 2.0 - 1.0;
            y[0] = (mBound->getPosition().getY() + currentCorner.getY().getValue() - lineThickness) / (double)windowHeight * 2.0 - 1.0;

            x[1] = (mBound->getPosition().getX() + nextCorner.getX().getValue() + lineThickness) / (double)windowWidth * 2.0 - 1.0;
            y[1] = (mBound->getPosition().getY() + nextCorner.getY().getValue() - sign* lineThickness) / (double)windowHeight * 2.0 - 1.0;

            x[2] = (mBound->getPosition().getX() + nextCorner.getX().getValue() + sign* lineThickness) / (double)windowWidth * 2.0 - 1.0;
            y[2] = (mBound->getPosition().getY() + nextCorner.getY().getValue() + lineThickness) / (double)windowHeight * 2.0 - 1.0;

            x[3] = (mBound->getPosition().getX() + currentCorner.getX().getValue() - lineThickness) / (double)windowWidth * 2.0 - 1.0;
            y[3] = (mBound->getPosition().getY() + currentCorner.getY().getValue() + sign* lineThickness) / (double)windowHeight * 2.0 - 1.0;

           for (int j = 0; j < Rectangle::CornerCount; j++)
            {
                y[j] = -y[j];
                vertices.push_back(x[j]);
                vertices.push_back(y[j]);
                colors.push_back((double)color.getRed()); // R
                colors.push_back((double)color.getGreen()); // G
                colors.push_back((double)color.getBlue()); // B
                colors.push_back((double)color.getAlpha()); // Alpha
            }
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getVertexBuffer());
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getColorBuffer());
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Draw
        glDrawArrays(GL_QUADS, 0, vertices.size() / 2);

        glBindVertexArray(0);
    }
    void Frame::fillRectangle(Color color, Rectangle bound)
    {
        std::vector<float> vertices;
        std::vector<float> colors;  // Add color information
        for (int i = 0; i <= Rectangle::CornerCount; ++i) {
            Point currentCorner = bound.getCorner(i % Rectangle::CornerCount);
            float x_outer = (mBound->getPosition().getX() + currentCorner.getX().getValue()) / (double)windowWidth * 2.0 - 1.0;
            float y_outer = (mBound->getPosition().getY() + currentCorner.getY().getValue()) / (double)windowHeight * 2.0 - 1.0;
            y_outer = -y_outer;
            vertices.push_back(x_outer);
            vertices.push_back(y_outer);
            colors.push_back((double)color.getRed()); // R
            colors.push_back((double)color.getGreen()); // G
            colors.push_back((double)color.getBlue()); // B
            colors.push_back((double)color.getAlpha()); // Alpha
        }
        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getVertexBuffer());
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getColorBuffer());
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Draw
        glDrawArrays(GL_QUADS, 0, vertices.size() / 2);

        glBindVertexArray(0);
    }
    void Frame::drawCircle(Color color, Rectangle bound, double stroke)
    {
        std::vector<float> vertices;
        const int segments = 100;
        const float lineThickness = stroke;  // Adjust this value for the desired line thickness
        Point midPoint = bound.getCenter();
        double width = bound.getWidth();
        double height = bound.getHeight();
        std::vector<float> colors;  // Add color information
        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * 3.14159265 * (static_cast<float>(i) + 0.5) / static_cast<float>(segments);

            // Outer point
            float x_outer = mBound->getPosition().getX() + midPoint.getX() + (width / 2 + lineThickness) * glm::cos(theta);
            float y_outer = mBound->getPosition().getY() + midPoint.getY() + (height / 2 + lineThickness) * glm::sin(theta);
            x_outer = x_outer / (double)windowWidth * 2.0 - 1.0;
            y_outer = y_outer / (double)windowHeight * 2.0 - 1.0;
            y_outer = -y_outer;
            vertices.push_back(x_outer);
            vertices.push_back(y_outer);

            // Inner point
            float x_inner = mBound->getPosition().getX() + midPoint.getX() + (width / 2 - lineThickness) * glm::cos(theta);
            float y_inner = mBound->getPosition().getY() + midPoint.getY() + (height / 2 - lineThickness) * glm::sin(theta);
            x_inner = x_inner / (double)windowWidth * 2.0 - 1.0;
            y_inner = y_inner / (double)windowHeight * 2.0 - 1.0;
            y_inner = -y_inner;
            vertices.push_back(x_inner);
            vertices.push_back(y_inner);

            colors.push_back((double)color.getRed()); // R
            colors.push_back((double)color.getGreen()); // G
            colors.push_back((double)color.getBlue()); // B
            colors.push_back((double)color.getAlpha()); // Alpha

            colors.push_back((double)color.getRed()); // R
            colors.push_back((double)color.getGreen()); // G
            colors.push_back((double)color.getBlue()); // B
            colors.push_back((double)color.getAlpha()); // Alpha
        }
        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getVertexBuffer());
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getColorBuffer());
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 2);
    }

    void Frame::fillCircle(Color color, Rectangle bound)
    {
        std::vector<float> vertices;
        const int segments = 100;
        Point midPoint = bound.getCenter();
        double width = bound.getWidth();
        double height = bound.getHeight();
        std::vector<float> colors;  // Add color information
        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * 3.14159265 * (static_cast<float>(i) + 0.5) / static_cast<float>(segments);

            // Outer point
            float x_outer = mBound->getPosition().getX() + midPoint.getX() + (width / 2 ) * glm::cos(theta);
            float y_outer = mBound->getPosition().getY() + midPoint.getY() + (height / 2 ) * glm::sin(theta);
            x_outer = x_outer / (double)windowWidth * 2.0 - 1.0;
            y_outer = y_outer / (double)windowHeight * 2.0 - 1.0;
            y_outer = -y_outer;
            vertices.push_back(x_outer);
            vertices.push_back(y_outer);

            // Inner point
            float x_inner = mBound->getPosition().getX() + midPoint.getX();
            float y_inner = mBound->getPosition().getY() + midPoint.getY();
            x_inner = x_inner / (double)windowWidth * 2.0 - 1.0;
            y_inner = y_inner / (double)windowHeight * 2.0 - 1.0;
            y_inner = -y_inner;
            vertices.push_back(x_inner);
            vertices.push_back(y_inner);

            colors.push_back((double)color.getRed()); // R
            colors.push_back((double)color.getGreen()); // G
            colors.push_back((double)color.getBlue()); // B
            colors.push_back((double)color.getAlpha()); // Alpha

            colors.push_back((double)color.getRed()); // R
            colors.push_back((double)color.getGreen()); // G
            colors.push_back((double)color.getBlue()); // B
            colors.push_back((double)color.getAlpha()); // Alpha
        }
        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getVertexBuffer());
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getColorBuffer());
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 2);
    }
    void Frame::drawArc(Color color, Rectangle bound, double stroke, double startDegree, double endDegree, Direction direction)
    {
        std::vector<float> vertices;
        const int segments = 100;
        const float lineThickness = stroke;  // Adjust this value for the desired line thickness
        Point midPoint = bound.getCenter();
        double width = bound.getWidth();
        double height = bound.getHeight();
        if (direction == ClockWise)
        {
            if (endDegree > startDegree)
            {
                endDegree -= 360;
            }
        }
        else
        {
            if (startDegree  > endDegree)
            {
                startDegree -= 360;
            }
        }
        double startRadian = -startDegree / 180.0 * 3.14159265;
        double endRadian = -endDegree / 180.0 * 3.14159265;

        std::vector<float> colors;  // Add color information
        for (int i = 0; i <= segments; ++i) {
            float ratio = (static_cast<float>(i)) / static_cast<float>(segments);
            float theta = startRadian + ratio * (endRadian-startRadian);

            // Outer point
            float x_outer = mBound->getPosition().getX() + midPoint.getX() + (width / 2 + lineThickness) * glm::cos(theta);
            float y_outer = mBound->getPosition().getY() + midPoint.getY() + (height / 2 + lineThickness) * glm::sin(theta);
            x_outer = x_outer / (double)windowWidth * 2.0 - 1.0;
            y_outer = y_outer / (double)windowHeight * 2.0 - 1.0;
            y_outer = -y_outer;
            vertices.push_back(x_outer);
            vertices.push_back(y_outer);

            // Inner point
            float x_inner = mBound->getPosition().getX() + midPoint.getX() + (width / 2 - lineThickness) * glm::cos(theta);
            float y_inner = mBound->getPosition().getY() + midPoint.getY() + (height / 2 - lineThickness) * glm::sin(theta);
            x_inner = x_inner / (double)windowWidth * 2.0 - 1.0;
            y_inner = y_inner / (double)windowHeight * 2.0 - 1.0;
            y_inner = -y_inner;
            vertices.push_back(x_inner);
            vertices.push_back(y_inner);

            colors.push_back((double)color.getRed()); // R
            colors.push_back((double)color.getGreen()); // G
            colors.push_back((double)color.getBlue()); // B
            colors.push_back((double)color.getAlpha()); // Alpha

            colors.push_back((double)color.getRed()); // R
            colors.push_back((double)color.getGreen()); // G
            colors.push_back((double)color.getBlue()); // B
            colors.push_back((double)color.getAlpha()); // Alpha
        }
        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getVertexBuffer());
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


        glBindBuffer(GL_ARRAY_BUFFER, mWindow->getColorBuffer());
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
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