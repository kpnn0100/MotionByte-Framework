/*
    MotionByte Framework
    Created by: kpnn0100 / Doan Le Nhat Nam
    Version: 1.0
*/

#pragma once
#include <vector>
#include <memory>
#include "Rectangle.h"
#include "../Graphic/Frame.h"
#include "Event/MouseEvenHandler.h"
#include "../Property/PropertyHeader.h"
#include "TransformerMachine.h"
#include <string>

namespace MotionByte {

    /**
     * @brief The Segment class represents a rectangular area within a graphical window,
     *        providing a foundation for organizing and interacting with graphical elements.
     *        It inherits from MouseEvenHandler to handle mouse events.
     */
    class Segment : public MouseEvenHandler, public TransformerMachine {
    private:
        friend class Frame;
    protected:
        PropertyManager mPropertyManager;                     ///< Manages properties associated with the segment.
        Segment* mParent;                                     ///< Pointer to the parent segment.
        std::vector<std::shared_ptr<Segment>> mChildrenList;   ///< List of child segments.
        Rectangle mBound;                                     ///< Rectangle defining the bounds of the segment.
        Frame mainFrame;                                       ///< Frame associated with the segment for rendering.
        Window* mTopParent;                                    ///< Pointer to the top-level parent window.
        std::string mSegmentTypeName;                          ///< Name identifying the segment type.
        int mSegmentTypeID;                                    ///< Unique identifier for the segment type.
        bool mIsLimited;
        bool mIsChildLimited;
        // Mouse event handling functions
        virtual void recursivePress(Point point, bool& handled) override;
        virtual void recursiveRelease(Point point, bool& handled) override;
        virtual void recursiveMove(Point point) override;
        virtual void recursiveAction(Point point, bool& handled, MouseButton button, MouseAction mouseEvent) override;
        virtual void recursiveScroll(Point point, double xValue, double yValue);
    public:
        enum PropertyID {
            X,               ///< X-coordinate of the top-left corner of the segment.
            Y,               ///< Y-coordinate of the top-left corner of the segment.
            Width,           ///< Width of the segment.
            Height,          ///< Height of the segment.
            PropertyCount    ///< Number of properties.
        };

        /**
         * @brief Constructor for Segment.
         *        Initializes the properties associated with the segment.
         */
        Segment();

        /**
         * @brief Getter for the property manager associated with the segment.
         * @return Reference to the PropertyManager.
         */
        PropertyManager& getSegmentPropertyManager();

        /**
         * @brief Sets the top-left position of the segment.
         * @param point Point specifying the top-left position.
         */
        void setTopLeftPosition(Point point);

        /**
         * @brief Sets the center position of the segment.
         * @param point Point specifying the center position.
         */
        void setCenterPosition(Point point);

        /**
         * @brief Sets the bounding rectangle of the segment.
         * @param bound Rectangle defining the bounds of the segment.
         */
        void setBound(Rectangle bound);

        /**
         * @brief Gets the bounding rectangle of the segment.
         * @return Rectangle defining the bounds of the segment.
         */
        Rectangle getBound() const;

        /**
         * @brief Gets the local bounding rectangle of the segment.
         * @return Local bounding rectangle with respect to the segment's parent.
         */
        Rectangle getLocalBound() const;

        /**
         * @brief Adds a child segment to the segment's list of children.
         * @param segment Shared pointer to the child segment.
         */
        void addSegment(std::shared_ptr<Segment> segment);

        /**
         * @brief Triggers the paint event for the segment and its children.
         */
        void triggerPaint();

        /**
         * @brief Pure virtual function for painting the segment.
         *        This function must be implemented by derived classes.
         * @param frame Frame used for rendering graphics within the segment.
         */
        virtual void paint(Frame& frame) = 0;

        void setIsLimited(bool limited);
        void setIsChildLimited(bool limited);
    };
}
