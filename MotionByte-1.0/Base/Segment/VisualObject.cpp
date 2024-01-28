#include "VisualObject.h"
namespace MotionByte
{
    VisualObject::VisualObject() : mObjectPropertyList(PropertyCount), mObjectColorList(ColorCount)
    {
        mObjectPropertyList.setPropertyPointer(X, &mBound.getPosition().getX());
        mObjectPropertyList.setPropertyPointer(Y, &mBound.getPosition().getY());
        mObjectPropertyList.setPropertyPointer(Width, &mBound.getWidth());
        mObjectPropertyList.setPropertyPointer(Height, &mBound.getWidth());
        mObjectPropertyList.setPropertyPointer(Radius, &mInsiderObject->mRadius);
        mObjectPropertyList.setPropertyPointer(BorderThickness, &mInsiderObject->mBorderThickness);
        mObjectPropertyList.setPropertyPointer(Radius, &mInsiderObject->mRadius);
        mObjectColorList.setColorPointer(ObjectColorID::Background, &mInsiderObject->mBackgroundColor);
        mObjectColorList.setColorPointer(ObjectColorID::Border, &mInsiderObject->mBorderColor);
        addSegment(mInsiderObject);
    }
    void VisualObject::InsiderObject::onParentChanged()
    {
        bindBoundToParent();
    }
    VisualObject::InsiderObject::InsiderObject()
    {
        mRadius.setMin(0.0);
        mBorderThickness.setMin(0.0);
    }
    void VisualObject::InsiderObject::paint(Frame& frame)
    {
        frame.fillRoundedRectangle(mBackgroundColor, getLocalBound(), mRadius);
        frame.drawRoundedRectangle(mBorderColor, getLocalBound(), mRadius, mBorderThickness);
    }

    PropertyManager& VisualObject::getObjectProperties()
    {
        return mObjectPropertyList;
    }
    ColorManager& VisualObject::getObjectColors()
    {
        return mObjectColorList;
    }

    void VisualObject::paint(Frame& frame)
    {

    }

    void VisualObject::setBorderThickness(double thickness)
    {
        mInsiderObject->mBorderThickness = thickness;
    }

    void VisualObject::setBorderColor(Color color)
    {
        mInsiderObject->mBorderColor.setColor(color);
    }

    void VisualObject::setRadius(double radius)
    {
        mInsiderObject->mRadius = radius;
    }

    void VisualObject::setBackgroundColor(Color color)
    {
        mInsiderObject->mBackgroundColor.setColor(color);
    }

}