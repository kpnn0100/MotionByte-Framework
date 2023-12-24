#include "Segment.h"
namespace pertyG
{
    Segment::Segment() : mainFrame(&mBound)
    {

    }
    void Segment::setBound(Rectangle bound)
    {
        mBound = bound;
    }
    void Segment::addSegment(std::shared_ptr<Segment> segment)
    {
        mChildrenList.push_back(segment);
    }
    void Segment::triggerPaint()
    {
        paint();
        for (auto children : mChildrenList)
        {
            children->triggerPaint();
        }
    }
}