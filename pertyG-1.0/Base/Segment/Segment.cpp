#include "Segment.h"
namespace pertyG
{
    void Segment::recursiveClick(Point point, bool& handled)
    {
        for (auto& segment : mChildrenList)
        {
            if (segment->mBound.isInside(point))
            {
                Point corner = segment->mBound.getCorner(Rectangle::TopLeft);
                Point newClickedPoint = point.withOffset(Point(-corner.getX().getValue(), -corner.getY().getValue()));
                segment->clickAt(newClickedPoint);

            }
        }
    }
    Segment::Segment() : mainFrame(&mBound), mPropertyManager(PropertyCount)
    {
        mPropertyManager.setPropertyPointer(X, &mBound.getPosition().getX());
        mPropertyManager.setPropertyPointer(Y, &mBound.getPosition().getY());
        mPropertyManager.setPropertyPointer(Width, &mBound.getWidth());
        mPropertyManager.setPropertyPointer(Height, &mBound.getWidth());
    }
    PropertyManager& Segment::getSegmentPropertyManager()
    {
        return mPropertyManager;
    }
    void Segment::setTopLeftPosition(Point point)
    {
        mBound.getPosition().setPosition(0, point.getX());
        mBound.getPosition().setPosition(1, point.getY());
    }
    void Segment::setCenterPosition(Point point)
    {
        mBound.getPosition().setPosition(0, point.getX()- mBound.getWidth()/2.0);
        mBound.getPosition().setPosition(1, point.getY() - mBound.getHeight() / 2.0);
    }
    void Segment::setBound(Rectangle bound)
    {
        mBound = bound;
    }
    Rectangle Segment::getBound() const
    {
        return mBound;
    }
    Rectangle Segment::getLocalBound() const
    {
        Rectangle newBound = mBound;
        newBound.setPosition(Point(0, 0));
        return newBound;
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