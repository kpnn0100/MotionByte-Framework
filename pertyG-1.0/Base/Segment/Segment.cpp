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
    Rectangle& Segment::getBound()
    {
        return mBound;
    }
    void Segment::addSegment(std::shared_ptr<Segment> segment)
    {
        segment->mParent = this;
        segment->mTopParent = mTopParent;
        segment->mainFrame.setWindow(mTopParent);
        mChildrenList.push_back(segment);
    }
    void Segment::triggerPaint()
    {
        paint(mainFrame);
        for (auto children : mChildrenList)
        {
            children->triggerPaint();
        }
    }
}