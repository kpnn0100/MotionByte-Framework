#pragma once
#include <vector>
#include <memory>
#include "Rectangle.h"
#include "../Graphic/Frame.h"
#include "Event/MouseEvenHandler.h"
#include <string>
namespace pertyG
{
    class Window;
    class Segment : public MouseEvenHandler
    {
    protected:
        Segment* mParent;
        std::vector<std::shared_ptr<Segment>> mChildrenList;
        Rectangle mBound;
        Frame mainFrame;
        Window* mTopParent;
        std::string mSegmentTypeName;
        int mSegmentTypeID;
        virtual void recursiveClick(Point point, bool& handled) override;
    public:
        Segment();
        void setBound(Rectangle bound);
        Rectangle& getBound();

        void addSegment(std::shared_ptr<Segment> segment);
        void triggerPaint();
        virtual void paint(Frame &frame) = 0;
    };
}