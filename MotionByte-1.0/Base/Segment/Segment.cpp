#include "Segment.h"
namespace MotionByte
{
    void Segment::recursivePress(Point point, bool& handled)
    {
        for (auto& segment : mChildrenList)
        {
            Point corner = segment->mBound.getCorner(Rectangle::TopLeft);
            Point newClickedPoint = point.withOffset(Point(-corner.getX().getValue(), -corner.getY().getValue()));
            if (segment->mBound.isInside(point))
            {
                segment->pressAt(newClickedPoint);
            }
            else
            {
                segment->clickOutside(newClickedPoint);
            }
        }
    }
    void Segment::recursiveRelease(Point point, bool& handled)
    {
        for (auto& segment : mChildrenList)
        {
            if (segment->mBound.isInside(point))
            {
                Point corner = segment->mBound.getCorner(Rectangle::TopLeft);
                Point releasedPoint = point.withOffset(Point(-corner.getX().getValue(), -corner.getY().getValue()));
                segment->releaseAt(releasedPoint);
            }
            else if (segment->isPressing())
            {
                Point corner = segment->mBound.getCorner(Rectangle::TopLeft);
                Point releasedPoint = point.withOffset(Point(-corner.getX().getValue(), -corner.getY().getValue()));
                segment->releaseAt(releasedPoint);
            }
        }
    }
    void Segment::recursiveMove(Point point)
    {
        for (auto& segment : mChildrenList)
        {
            Point corner = segment->mBound.getCorner(Rectangle::TopLeft);
            Point newPoint = point.withOffset(Point(-corner.getX().getValue(), -corner.getY().getValue()));
            segment->mouseMove(newPoint);
            if (segment->mBound.isInside(point))
            {
                segment->mouseEnter();
            }
            else
            {
                if (segment->isHoverOn())
                {
                    segment->mouseExit();
                }
            }
        }
    }
    void Segment::recursiveAction(Point point, bool& handled, MouseButton button, MouseAction mouseEvent)
    {
        for (auto& segment : mChildrenList)
        {
            if (segment->mBound.isInside(point))
            {
                Point corner = segment->mBound.getCorner(Rectangle::TopLeft);
                Point newClickedPoint = point.withOffset(Point(-corner.getX().getValue(), -corner.getY().getValue()));
                segment->mouseAction(newClickedPoint, button,mouseEvent);
            }
        }
    }
    void Segment::recursiveScroll(Point point, double xValue, double yValue)
    {
        for (auto& segment : mChildrenList)
        {
            if (segment->mBound.isInside(point))
            {
                Point corner = segment->mBound.getCorner(Rectangle::TopLeft);
                Point newClickedPoint = point.withOffset(Point(-corner.getX().getValue(), -corner.getY().getValue()));
                segment->scrollAt(newClickedPoint, xValue, yValue);
            }
        }
    }
    Segment::Segment() : mainFrame(this),
        mPropertyManager(PropertyCount),
        mIsChildLimited(false),
        mIsLimited(false),
        mParent(nullptr),
        mTopParent(nullptr)
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
        for (auto &children : mChildrenList)
        {
            children->triggerPaint();
        }
    }
    void Segment::setIsLimited(bool limited)
    {
        mIsLimited = limited;
    }
    void Segment::setIsChildLimited(bool limited)
    {
        mIsChildLimited = limited;
    }
}