/*
    MotionByte Framework
    Created by: kpnn0100 / Doan Le Nhat Nam
    Version: 1.0
*/

#pragma once
#include "Point.h"
#include "../Property/Property.h"

namespace MotionByte {

    /**
     * @brief The Rectangle class represents a rectangle with a position, size, and various methods
     *        for manipulation and querying.
     */
    class Rectangle {
        Point mPosition;         ///< Position of the top-left corner of the rectangle.
        Property mSize[2];       ///< Size of the rectangle (width and height).

    public:
        enum Corner {
            TopLeft = 0,        ///< Top-left corner of the rectangle.
            TopRight,           ///< Top-right corner of the rectangle.
            BottomRight,        ///< Bottom-right corner of the rectangle.
            BottomLeft,         ///< Bottom-left corner of the rectangle.
            CornerCount         ///< Number of corners.
        };

        /**
         * @brief Default constructor for Rectangle.
         *        Initializes the rectangle with a position at (0,0) and size (0,0).
         */
        Rectangle();

        Rectangle(double x1, double y1, double x2, double y2);
        /**
         * @brief Parameterized constructor for Rectangle.
         * @param position Point specifying the top-left corner of the rectangle.
         * @param width Width of the rectangle.
         * @param height Height of the rectangle.
         */
        Rectangle(Point position, double width, double height);

        /**
         * @brief Copy constructor for Rectangle.
         * @param other Reference to another Rectangle object.
         */
        Rectangle(const Rectangle& other);

        /**
         * @brief Copy assignment operator for Rectangle.
         * @param other Reference to another Rectangle object.
         * @return Reference to the current Rectangle object.
         */
        Rectangle operator=(const Rectangle& other);

        // Getter methods

        /**
         * @brief Getter for the position of the top-left corner of the rectangle.
         * @return Reference to the Point representing the position.
         */
        Point& getPosition();

        /**
         * @brief Getter for a specific corner of the rectangle.
         * @param cornerID Identifier of the corner (TopLeft, TopRight, BottomRight, BottomLeft).
         * @return Point representing the specified corner.
         */
        Point getCorner(int cornerID);

        /**
         * @brief Getter for the center point of the rectangle.
         * @return Point representing the center of the rectangle.
         */
        Point getCenter();

        /**
         * @brief Getter for the local center point of the rectangle.
         * @return Point representing the local center of the rectangle.
         */
        Point getLocalCenter();

        /**
         * @brief Getter for the width property of the rectangle.
         * @return Reference to the Property representing the width.
         */
        Property& getWidth();

        /**
         * @brief Getter for the height property of the rectangle.
         * @return Reference to the Property representing the height.
         */
        Property& getHeight();

        /**
         * @brief Creates a new rectangle with a different size while keeping the center constant.
         * @param newWidth New width for the rectangle.
         * @param newHeight New height for the rectangle.
         * @return Rectangle with the updated size and the center unchanged.
         */
        Rectangle withSizeKeepCenter(double newWidth, double newHeight);

        /**
         * @brief Creates a new rectangle with a different top-left position.
         * @param position New top-left position for the rectangle.
         * @return Rectangle with the updated top-left position.
         */
        Rectangle withTopLeft(Point position);

        /**
         * @brief Creates a new rectangle with a different center position.
         * @param position New center position for the rectangle.
         * @return Rectangle with the updated center position.
         */
        Rectangle withCenter(Point position);

        /**
         * @brief Checks if a given point is inside the rectangle.
         * @param point Point to check.
         * @return True if the point is inside the rectangle, otherwise false.
         */
        bool isInside(Point point);

        // Setter methods

        /**
         * @brief Sets the top-left position of the rectangle.
         * @param position New top-left position.
         */
        void setPosition(const Point& position);

        /**
         * @brief Sets the size of the rectangle.
         * @param width New width for the rectangle.
         * @param height New height for the rectangle.
         */
        void setSize(double width, double height);

        // Move the rectangle by a certain offset

        /**
         * @brief Moves the rectangle by a specified offset.
         * @param vector Offset to move the rectangle by.
         */
        void moveBy(Point vector);

        // Resize the rectangle by a certain factor

        /**
         * @brief Resizes the rectangle by a specified factor.
         * @param widthFactor Factor to resize the width by.
         * @param heightFactor Factor to resize the height by.
         */
        void resizeBy(double widthFactor, double heightFactor);
    };
}