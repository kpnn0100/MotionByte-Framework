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
    void VisualObject::InsiderObject::paint(Frame& frame)
    {
        frame.fillRoundedRectangle(mBackgroundColor, getLocalBound(), mRadius);
        frame.drawRoundedRectangle(mBackgroundColor, getLocalBound(), mRadius, mBorderThickness);
    }
    PropertyManager& VisualObject::getObjectProperties()
    {
        return mObjectPropertyList;
    }
    ColorManager& VisualObject::getObjectColors()
    {
        return mObjectColorList;
    }

}