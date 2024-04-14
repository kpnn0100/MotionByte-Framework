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
        mIsChildLimited(false),
        mIsLimited(false),
        mParent(nullptr),
        mTopParent(nullptr),
        mOpacity(1.0)
    {

    }
    Segment::~Segment()
    {
        detachFromParent();
    }
    Property& Segment::getOpacity()
    {
        return mOpacity;
    }
    void Segment::setOpacity(double opacity)
    {
         mOpacity = clamp(opacity, 0.0, 1.0);
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
    Rectangle & Segment::getBound()
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
        if (segment->mParent)
        {
            //remove that segment from another parent first
            return;
        }
        segment->setParent(this);
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
    double Segment::getOpacityFromOrigin()
    {
        return mParent ? mOpacity.getValue()*mParent->getOpacityFromOrigin() : mOpacity.getValue();
    }
    Point Segment::getOffsetFromOrigin()
    {
        return mParent ? mBound.getPosition() + mParent->getOffsetFromOrigin() : mBound.getPosition();
    }
    void Segment::onParentChanged()
    {
    }
    void Segment::detachFromParent()
    {
        if (mParent == nullptr)
        {
            //No parent available
            return;
        }

        mTopParent = nullptr;
        mainFrame.setWindow(nullptr);
        setParent(nullptr);


    }
    void Segment::bindBoundTo(std::weak_ptr<Segment> target)
    {
        auto targetSharedPtr = target.lock();
        mBound.getWidth().bind([target]
            {
                if (auto targetSharedPtr = target.lock())
                {
                    return targetSharedPtr->mBound.getWidth().getValue();
                }
                else
                {
                    return 0.0;
                }

            });
        mBound.getHeight().bind([target]
            {
                if (auto targetSharedPtr = target.lock())
                {
                    return targetSharedPtr->mBound.getHeight().getValue();
                }
                else
                {
                    return 0.0;
                }

            });
        if (targetSharedPtr->mParent != mParent)
        {
            debug("Cannot bind to non sibling");
            return;
        }
        mBound.getPosition().getX().bind([target]
            {
                if (auto targetSharedPtr = target.lock())
                {
                    return targetSharedPtr->mBound.getPosition().getX().getValue();
                }
                else
                {
                    return 0.0;
                }

            });
        mBound.getPosition().getY().bind([target]
            {
                if (auto targetSharedPtr = target.lock())
                {
                    return targetSharedPtr->mBound.getPosition().getY().getValue();
                }
                else
                {
                    return 0.0;
                }

            });
    }
    void Segment::bindBoundToParent()
    {
        if (mParent == nullptr)
        {
            return;
        }
        mBound.getWidth().bind([this]
            {
                return this->mParent->mBound.getWidth().getValue();
            });
        mBound.getHeight().bind([this]
            {
                return this->mParent->mBound.getHeight().getValue();
            });
        mBound.getPosition().getX().bind([this]
            {
                return 0.0;
            });
        mBound.getPosition().getY().bind([this]
            {
                return 0.0;
            });
    }
    void Segment::setParent(Segment* parent)
    {
        mParent = parent;
        onParentChanged();
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