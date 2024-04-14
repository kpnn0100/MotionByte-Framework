#include "CheckBox.h"
namespace MotionByte
{
    Color &CheckBox::getInsideTickColor()
    {
        return mInsideTickColor;
    }
    void CheckBox::setInsideTickColor(Color insideTickColor)
    {
        mInsideTickColor = insideTickColor;
    }
    double CheckBox::getInsideTickSize()
    {
        return mInsideTickSize;
    }
    void CheckBox::setInsideTickSize(double insideTickSize)
    {
        mInsideTickSize = insideTickSize;
    }
    void CheckBox::paint(Frame &frame)
    {
    }
    CheckBox::CheckBox()
    {
        mInsideTick = std::make_shared<VisualObject>();
        mInsideTickColor = DEFAULT_TICK_COLOR;
        mInsideTickSize = DEFAULT_TICK_SIZE;
        setRadius(DEFAULT_TICK_SIZE/2);
        mInsideTick->getObjectProperties()
            .setInterpolator(VisualObject::Radius,
                InterpolatorFactory::createSmooth(160,80));
        mInsideTickSizeProperty.setInterpolator(InterpolatorFactory::createSmooth(1280,640));
        mInsideTick->setBackgroundColor(DEFAULT_TICK_COLOR);
        mInsideTick->setRadius(DEFAULT_TICK_SIZE*2);
        mInsideTick->getBound().getWidth().bind(
            [this]() -> double
            {
                return mInsideTickSizeProperty.getValue();
            }
        );
        mInsideTick->getBound().getHeight().bind(
            [this]() -> double
            {
                return mInsideTickSizeProperty.getValue();
            }
        );
        mInsideTick->getBound().getPosition().getX().bind
        (
            [this]() -> double
            {
                return getBound().getWidth()/2 - mInsideTickSizeProperty.getValue() / 2;
            }
        );
        mInsideTick->getBound().getPosition().getY().bind
        (
            [this]() -> double
            {
                return getBound().getHeight()/2 - mInsideTickSizeProperty.getValue() / 2;
            }
        );
        addSegment(mInsideTick);
        setBound(Rectangle(Point(0,0),DEFAULT_CHECK_BOX_SIZE,DEFAULT_CHECK_BOX_SIZE));
        setBorderColor(DEFAULT_TICK_COLOR);
        setBorderThickness(DEFAULT_THICKNESS);
    }
    CheckBox::~CheckBox()
    {
    }
    void CheckBox::onButtonStateChanged(ButtonState state)
    {
        if (state == ButtonState::Pressed)
        {
            mInsideTickSizeProperty = mInsideTickSize;
            mInsideTick->setRadius(0);
        }
        else
        {
            mInsideTickSizeProperty = 0;
            mInsideTick->setRadius(mInsideTickSize/2);
        }
    }
}