#pragma once
#include <vector>
#include <memory>
#include "Rectangle.h"
#include "../Graphic/Frame.h"
#include "Event/MouseEvenHandler.h"
#include "../Property/PropertyHeader.h"
#include <string>
namespace MotionByte
{
    class Window;
    class Segment : public MouseEvenHandler
    {
    protected:
        PropertyManager mPropertyManager;
        Segment* mParent;
        std::vector<std::shared_ptr<Segment>> mChildrenList;
        Rectangle mBound;
        Frame mainFrame;
        Window* mTopParent;
        std::string mSegmentTypeName;
        int mSegmentTypeID;
        virtual void recursivePress(Point point, bool& handled) override;
        virtual void recursiveRelease(Point point, bool& handled) override;
        virtual void recursiveMove(Point point) override;
    public:
        enum PropertyID
        {
            X,
            Y,
            Width,
            Height,
            PropertyCount
        };
        Segment();
        PropertyManager& getSegmentPropertyManager();
        void setTopLeftPosition(Point point);
        void setCenterPosition(Point point);
        void setBound(Rectangle bound);
        Rectangle getBound() const;
        Rectangle getLocalBound() const;
        void addSegment(std::shared_ptr<Segment> segment);
        void triggerPaint();
        virtual void paint(Frame &frame) = 0;
    };
}