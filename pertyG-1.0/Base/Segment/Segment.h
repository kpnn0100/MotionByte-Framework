#pragma once
#include <vector>
#include <memory>
#include "Rectangle.h"
#include "../Graphic/Frame.h"
namespace pertyG
{
    class Window;
    class Segment
    {
    protected:
        Segment* mParent;
        std::vector<std::shared_ptr<Segment>> mChildrenList;
        Rectangle mBound;
        Frame mainFrame;
        Window* mTopParent;
    public:
        Segment();
        void setBound(Rectangle bound);
        Rectangle& getBound();

        void addSegment(std::shared_ptr<Segment> segment);
        void triggerPaint();
        virtual void paint(Frame &frame) = 0;
    };
}