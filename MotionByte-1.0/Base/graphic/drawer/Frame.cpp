#include "Frame.h"
#include "../window/Window.h"
#include <math.h>
#include <float.h>
namespace MotionByte
{
    int Frame::windowWidth = 0;
    int Frame::windowHeight = 0;
    Color Frame::withEffect(Color color)
    {
        Color newColor = color;
        newColor.setAlpha(newColor.getAlpha().getValue() * mSegment->getOpacityFromOrigin());
        return newColor;
    }
    void Frame::updateUniformForShape()
    {
        Point offset;
        if (mSegment->mParent != nullptr)
        {
            offset = mSegment->mParent->getOffsetFromOrigin();
        }
        auto segmentBound = mSegment->getBound();
        Point topLeft = offset+ segmentBound.getCorner(Rectangle::TopLeft);
        Point bottomRight = offset+ segmentBound.getCorner(Rectangle::BottomRight);

        //limit to local border
        Point topLeftLimit = offset + segmentBound.getCorner(Rectangle::TopLeft);
        Point bottomRightLimit = offset + segmentBound.getCorner(Rectangle::BottomRight);

        float xMin = topLeftLimit.getX();
        float xMax = bottomRightLimit.getX();
        float yMin = topLeftLimit.getY();
        float yMax = bottomRightLimit.getY();
        Point parentTopLeftLimit;
        Point parentBottomRightLimit;
        if (mSegment->mParent != nullptr)
        {
            parentTopLeftLimit = offset;
            parentBottomRightLimit = offset + Point(mSegment->mParent->getBound().getWidth(),
                mSegment->mParent->getBound().getHeight());
        }
        float parentxMin = parentTopLeftLimit.getX();
        float parentxMax = parentBottomRightLimit.getX();
        float parentyMin = parentTopLeftLimit.getY();
        float parentyMax = parentBottomRightLimit.getY();

        float renderXMin = 0.0;
        float renderXMax = FLT_MAX;
        float renderYMin = 0.0;
        float renderYMax = FLT_MAX;
        if (mSegment->mIsLimited)
        {
            renderXMin = xMin;
            renderXMax = xMax;
            renderYMin = yMin;
            renderYMax = yMax;
        }
        if (mSegment->mParent != nullptr)
        {
            if (mSegment->mParent->mIsChildLimited)
            {
                if (parentxMin > renderXMin)
                {
                    renderXMin = parentxMin;
                }

                if (parentxMax < renderXMax)
                {
                    renderXMax = parentxMax;
                }

                if (parentyMin > renderYMin)
                {
                    renderYMin = parentyMin;
                }

                if (parentyMax < renderYMax)
                {
                    renderYMax = parentyMax;
                }
            }
        }
        ShapeManager::instance().setBoundAndOffset(
            Rectangle(renderXMin, renderYMin, renderXMax, renderYMax),
            topLeft);
    }
    // Constructor that takes a std::shared_ptr<GLFWwindow>
    Frame::Frame(Segment* segment)
    {
        mSegment = segment;
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
        if (color.getAlpha() == 0.0)
            return;
        fillRectangle(color, mSegment->getLocalBound());
	}

    void Frame::drawRectangle(Color color, Rectangle bound, double stroke)
    {
        if (color.getAlpha() == 0.0)
            return;
        VertexList vertices;
        const float lineThickness = stroke/2;  // Adjust this value for the desired line thickness
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
            x[0] = (currentCorner.getX().getValue() - sign* lineThickness);
            y[0] = (currentCorner.getY().getValue() - lineThickness) ;

            x[1] = (nextCorner.getX().getValue() + lineThickness);
            y[1] = ( nextCorner.getY().getValue() - sign* lineThickness) ;

            x[2] = (nextCorner.getX().getValue() + sign* lineThickness) ;
            y[2] = ( nextCorner.getY().getValue() + lineThickness);

            x[3] = (currentCorner.getX().getValue() - lineThickness);
            y[3] = ( currentCorner.getY().getValue() + sign* lineThickness);

           for (int j = 0; j < Rectangle::CornerCount; j++)
            {
                vertices.addVertex(x[j], y[j]);
            }
        }
        updateUniformForShape();
        ShapeManager::instance().prepareBuffer(vertices, withEffect(color));
        // Draw
        glDrawArrays(GL_QUADS, 0, vertices.sizeInFloat() / 2);
    }
    void Frame::fillRectangle(Color color, Rectangle bound)
    {
        if (color.getAlpha() == 0.0)
            return;
        VertexList vertices;
        for (int i = 0; i <= Rectangle::CornerCount; ++i) {
            Point currentCorner = bound.getCorner(i % Rectangle::CornerCount);
            float x_outer = currentCorner.getX().getValue();
            float y_outer = currentCorner.getY().getValue();
            vertices.addVertex(x_outer, y_outer);
        }
        updateUniformForShape();
        ShapeManager::instance().prepareBuffer(vertices, withEffect(color));

        // Draw
        glDrawArrays(GL_QUADS, 0, vertices.sizeInFloat() / 2);
    }
    void Frame::fillPolygon(Color color, std::vector<Point> pointList)
    {
        if (color.getAlpha() == 0.0)
            return;
        VertexList vertices;
        std::vector<float> colors;  // Add color information
        int s = pointList.size();
        for (int i = 0; i <= s; ++i) {
            vertices.addVertex(pointList[i%s].getX(),
                pointList[i % s].getY());

        }
        updateUniformForShape();
        ShapeManager::instance().prepareBuffer(vertices, withEffect(color));;
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.sizeInFloat() / 2);
    }
    void Frame::fillPolygon(Color color, Point origin, std::vector<Point> pointList)
    {
        if (color.getAlpha() == 0.0)
            return;
        VertexList vertices;
        std::vector<float> colors;  // Add color information
        int s = pointList.size();
        vertices.addVertex(origin.getX(), origin.getY());
        for (int i = 0; i < s; ++i) {
            vertices.addVertex(pointList[i%s].getX(),
                pointList[i % s].getY());
        }
        updateUniformForShape();
        ShapeManager::instance().prepareBuffer(vertices, withEffect(color));;
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.sizeInFloat() / 2);
    }
    void Frame::drawRoundedRectangle(Color color, Rectangle bound, double radius, double stroke)
    {
        if (color.getAlpha() == 0.0)
            return;
        //special case when only outside edge got bended

        if (radius > bound.getWidth() / 2.0)
        {
            radius = bound.getWidth() / 2.0;
        }
        if (radius > bound.getHeight() / 2.0)
        {
            radius = bound.getHeight() / 2.0;
        }
        //upper
        Rectangle upper;
        upper.setPosition(Point(std::max(radius,stroke) + bound.getCorner(bound.TopLeft).getX(),
            bound.getCorner(bound.TopLeft).getY()));
        upper.setSize(bound.getWidth() - std::max(radius, stroke) * 2, stroke);
        fillRectangle(color, upper);
        //below
        Rectangle below;
        below.setPosition(Point(std::max(radius, stroke) + bound.getCorner(bound.TopLeft).getX(),
            bound.getCorner(bound.TopLeft).getY() + bound.getHeight() - stroke));
        below.setSize(bound.getWidth() - std::max(radius, stroke) * 2, stroke);
        fillRectangle(color, below);
        //left
        Rectangle left;
        left.setPosition(Point(bound.getCorner(bound.TopLeft).getX(),
            bound.getCorner(bound.TopLeft).getY()+ std::max(radius, stroke)));
        left.setSize( stroke, bound.getHeight() - std::max(radius, stroke) * 2 );
        fillRectangle(color, left);
        //right
        Rectangle right;
        right.setPosition(Point(bound.getCorner(bound.TopRight).getX()-stroke,
            bound.getCorner(bound.TopRight).getY() + std::max(radius, stroke)));
        right.setSize(stroke, bound.getHeight() - std::max(radius, stroke) * 2);
        fillRectangle(color, right);
        //round corner
        Rectangle middleBound = bound;
        middleBound = middleBound.withSizeKeepCenter(middleBound.getWidth(), middleBound.getHeight() - radius * 2);
        Rectangle middle = middleBound;
        Rectangle rec[6];
        rec[0] = bound.withSizeKeepCenter
            (bound.getWidth(), bound.getHeight() - stroke * 2);
        rec[1] = bound.withSizeKeepCenter
            (bound.getWidth(), bound.getHeight() - radius * 2);
        rec[2] = bound.withSizeKeepCenter
            (bound.getWidth() - radius * 2, bound.getHeight() - radius * 2);
        rec[3] = bound.withSizeKeepCenter
            (bound.getWidth() - radius * 2, bound.getHeight());
        rec[4] = bound.withSizeKeepCenter
            (bound.getWidth() - stroke * 2, bound.getHeight());
        rec[5] = bound.withSizeKeepCenter
            (bound.getWidth() - stroke * 2, bound.getHeight() - stroke * 2);
        middle = middle.withSizeKeepCenter(middle.getWidth() - 2 * radius, middle.getHeight());
        for (int i = 0; i < 4; i++)
        {
            double startDegree = 180.0 - i * 90.0;
            double endDegree = 180.0 - (i + 1) * 90.0;//padding
            
            Point center = middle.getCorner(i);
            if (radius > stroke)
            {
                drawAnnularArc(color, center, radius - stroke, radius, startDegree, endDegree, ClockWise);
                continue;
            }
            std::vector<Point> pointList;
            for (int j = 0; j < 5; j++)
            {
                pointList.push_back(rec[j].getCorner(i));
            }
            fillPolygon(color,rec[5].getCorner(i), pointList);
            drawAnnularArc(color, center, 0, radius, startDegree, endDegree, ClockWise);
        }
    }
    void Frame::fillRoundedRectangle(Color color, Rectangle bound, double radius)
    {
        if (color.getAlpha() == 0.0)
            return;
        //Rectangle bound(ubound.getPosition(),
        //    ubound.getWidth().getValue(),
        //    ubound.getHeight().getValue());
        if (radius > bound.getWidth() / 2.0)
        {
            radius = bound.getWidth() / 2.0;
        }
        if (radius > bound.getHeight() / 2.0)
        {
            radius = bound.getHeight() / 2.0;
        }
        //draw middle
        Rectangle middleBound = bound;
        middleBound = middleBound.withSizeKeepCenter(middleBound.getWidth(), middleBound.getHeight() - radius * 2);
        fillRectangle(color, middleBound);
        //upper rec and below rec
        Rectangle upper;
        upper.setPosition(Point(radius+ bound.getCorner(bound.TopLeft).getX(),
            bound.getCorner(bound.TopLeft).getY()));
        upper.setSize(bound.getWidth() - radius * 2, radius);
        fillRectangle(color, upper);
        //below
        Rectangle below;
        below.setPosition(Point(radius + bound.getCorner(bound.TopLeft).getX(),
            bound.getCorner(bound.TopLeft).getY()+bound.getHeight()-radius));
        below.setSize(bound.getWidth() - radius * 2, radius);
        fillRectangle(color, below);
        //round corner
        Rectangle middle = middleBound;
        middle = middle.withSizeKeepCenter(middle.getWidth()-2*radius, middle.getHeight());
        for (int i = 0; i < 4; i++)
        {
            double startDegree = 180.0 - i * 90.0;
            double endDegree =  180.0 - (i + 1) * 90.0;//padding
            Point center = middle.getCorner(i);
            drawAnnularArc(color, center, 0.0, radius, startDegree, endDegree, ClockWise);
        }
    }
    void Frame::drawCircle(Color color, Rectangle bound, double stroke)
    {
        if (color.getAlpha() == 0.0)
            return;
        VertexList vertices;
        const int segments = bound.getWidth();
        const float lineThickness = stroke;  // Adjust this value for the desired line thickness
        Point midPoint = bound.getCenter();
        double width = bound.getWidth();
        double height = bound.getHeight();
        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * 3.14159265 * (static_cast<float>(i) + 0.5) / static_cast<float>(segments);

            // Outer point
            float x_outer = midPoint.getX() + (width / 2 + lineThickness) * std::cos(theta);
            float y_outer = midPoint.getY() + (height / 2 + lineThickness) * std::sin(theta);
            vertices.addVertex(x_outer, y_outer);

            // Inner point
            float x_inner = midPoint.getX() + (width / 2 - lineThickness) * std::cos(theta);
            float y_inner = midPoint.getY() + (height / 2 - lineThickness) * std::sin(theta);
            vertices.addVertex(x_inner, y_inner);
        }
        updateUniformForShape();
        ShapeManager::instance().prepareBuffer(vertices, withEffect(color));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.sizeInFloat() / 2);
    }

    void Frame::fillCircle(Color color, Rectangle bound)
    {
        if (color.getAlpha() == 0.0)
            return;
        VertexList vertices;
        const int segments = bound.getWidth();
        Point midPoint = bound.getCenter();
        double width = bound.getWidth();
        double height = bound.getHeight();

        std::vector<float> colors;  // Add color information

        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * 3.14159265 * (static_cast<float>(i) + 0.5) / static_cast<float>(segments);

            // Outer point
            float x_outer = midPoint.getX() + (width / 2 ) * glm::cos(theta);
            float y_outer = midPoint.getY() + (height / 2 ) * glm::sin(theta);
            vertices.addVertex(x_outer, y_outer);

            // Inner point
            float x_inner = midPoint.getX();
            float y_inner = midPoint.getY();
            vertices.addVertex(x_inner, y_inner);

        }
        updateUniformForShape();
        ShapeManager::instance().prepareBuffer(vertices, withEffect(color));;
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.sizeInFloat() / 2);
    }
    void Frame::fillCircle(Color color, Point center, double radius)
    {
        if (color.getAlpha() == 0.0)
            return;
        Rectangle bound;
        bound.setSize(radius*2.0, radius*2.0);
        bound = bound.withCenter(center);
        fillCircle(color, bound);
    }
    void Frame::drawArc(Color color, Rectangle bound, double stroke, double startDegree, double endDegree, Direction direction)
    {
        if (color.getAlpha() == 0.0)
            return;
        VertexList vertices;
        const int segments = bound.getWidth();
        const float lineThickness = stroke/2.0;  // Adjust this value for the desired line thickness
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
        for (int i = 0; i <= segments; ++i) {
            float ratio = (static_cast<float>(i)) / static_cast<float>(segments);
            float theta = startRadian + ratio * (endRadian-startRadian);

            // Outer point
            float x_outer = midPoint.getX() + (width / 2 + lineThickness) * std::cos(theta);
            float y_outer = midPoint.getY() + (height / 2 + lineThickness) * std::sin(theta);
            vertices.addVertex(x_outer, y_outer);

            // Inner point
            float x_inner = midPoint.getX() + (width / 2 - lineThickness) * std::cos(theta);
            float y_inner = midPoint.getY() + (height / 2 - lineThickness) * std::sin(theta);
            vertices.addVertex(x_inner, y_inner);
        }
        updateUniformForShape();
        ShapeManager::instance().prepareBuffer(vertices, withEffect(color));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.sizeInFloat() / 2);
    }
    void Frame::drawArc(Color color, Point center, double radius, double stroke, double startDegree, double endDegree, Direction direction)
    {
        if (color.getAlpha() == 0.0)
            return;
        Rectangle bound;
        bound.setSize(radius*2.0, radius*2.0);
        bound = bound.withCenter(center);
        drawArc(color, bound, stroke, startDegree, endDegree, direction);
    }
    void Frame::drawAnnularArc(Color color, Point center, double innerRadius, double outerRadius, double startDegree, double endDegree, Direction direction)
    {
        if (color.getAlpha() == 0.0)
            return;
        drawArc(color, center, (innerRadius + outerRadius) / 2.0, outerRadius - innerRadius, startDegree, endDegree, direction);
    }

    void Frame::drawText(Color color, Font &font, std::string text, Point position, double size)
    {
        if (color.getAlpha() == 0.0)
            return;
        FontManager::instance().RenderText(withEffect(color), font, text, position.getX(), position.getY(), size);
    }

    void Frame::drawText(Color color, Font &font, std::string text, double size, Rectangle bound, Align align)
    {
        if (color.getAlpha() == 0.0)
            return;
        Point coor = mSegment->getOffsetFromOrigin() + mSegment->getBound().getCorner(Rectangle::TopLeft);
        bound.moveBy(coor);
        FontManager::instance().RenderText(withEffect(color), font, text, size, bound, align);
    }

}