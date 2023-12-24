#pragma once
#include <vector>
#include <memory>
#include "Rectangle.h"
#include "../Graphic/Frame.h"
namespace pertyG
{
    class Segment
    {
    protected:
        std::vector<std::shared_ptr<Segment>> mChildrenList;
        Rectangle mBound;
        Frame mainFrame;
    public:
        Segment();
        void setBound(Rectangle bound);
        void addSegment(std::shared_ptr<Segment> segment);
        void triggerPaint();
        virtual void paint() = 0;
    };
}