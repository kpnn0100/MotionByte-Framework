#include "AbstractSlider.h"
namespace MotionByte
{
    AbstractSlider::AbstractSlider(float minValue, float maxValue, float value) :
    mMinValue(minValue), mMaxValue(maxValue), mStep(0),
    mDragType(DragType::Drag), mDragOrientation(DragOrientation::Horizontal)
    {
        setValue(value);
        mPercent.setPropertyName("slider::percent");
        mPercent.setInterpolator(InterpolatorFactory::createSmooth(50.0, 1.2));
    }
    void AbstractSlider::setOnValueChangedCallback(std::function<void(double value)> callback)
    {
        mOnValueChangedCallback = callback;
    }
    void AbstractSlider::setValue(float value)
    {
        if (value < mMinValue)
        {
            value = mMinValue;
        }
        if (value > mMaxValue)
        {
            value = mMaxValue;
        }
        if (mStep > 0)
        {
            value = (mMaxValue - mMinValue) / mStep * round((value - mMinValue) / (mMaxValue - mMinValue) * mStep) + mMinValue;
        }
        if (value == mValue)
        {
            return;
        }
        this->mValue = value;
        onValueChanged(value);
        if (mOnValueChangedCallback)
        {
            mOnValueChangedCallback(value);
        }
        setPercent((value - mMinValue) / (mMaxValue - mMinValue));
        
    }
    void AbstractSlider::setStep(int step)
    {
        this->mStep = step;
        updateValue();
    }
    int AbstractSlider::getStep()
    {
        return mStep;
    }
    float AbstractSlider::getValue()
    {
        return mValue;
    }
    void AbstractSlider::setMinValue(float minValue)
    {
        this->mMinValue = minValue;
        updateValue();
    }
    float AbstractSlider::getMinValue()
    {
        return mMinValue;
    }
    void AbstractSlider::setMaxValue(float maxValue)
    {
        this->mMaxValue = maxValue;
        updateValue();
    }
    float AbstractSlider::getMaxValue()
    {
        return mMaxValue;
    }
    void AbstractSlider::setRange(float minValue, float maxValue)
    {
        this->mMinValue = minValue;
        this->mMaxValue = maxValue;
        updateValue();
    }
    void AbstractSlider::setRange(float minValue, float maxValue, float value)
    {
        this->mMinValue = minValue;
        this->mMaxValue = maxValue;
        setValue(value);
    }
    void AbstractSlider::setDragOrientation(DragOrientation orientation)
    {
        this->mDragOrientation = mDragOrientation;
    }
    AbstractSlider::DragOrientation AbstractSlider::getDragOrientation()
    {
        return mDragOrientation;
    }
    void AbstractSlider::setDragType(DragType type)
    {
        this->mDragType = type;
    }
    AbstractSlider::DragType AbstractSlider::getDragType()
    {
        return mDragType;
    }
    void AbstractSlider::onPressed(Point point)
    {
        if (mDragType == DragType::Click)
        {
            if (mDragOrientation == DragOrientation::Horizontal)
            {
                double percent = (point.getX() - getLocalBound().getLeft()) / getBound().getWidth();
                setValue( percent* (mMaxValue - mMinValue) + mMinValue);
            }
            else if (mDragOrientation == DragOrientation::Vertical)
            {
                setValue((point.getY() - getLocalBound().getTop()) / getBound().getHeight() * (mMaxValue - mMinValue) + mMinValue);
            }
        }
        if (mDragType == DragType::Drag)
        {
            mValueAtPress = mValue;
        }
    }
    void AbstractSlider::onMouseDragged(Point point,Point origin)
    {
        if (mDragType == DragType::Drag)
        {
            if (mDragOrientation == DragOrientation::Horizontal)
            {
                float percent = (point.getX() - origin.getX()) / getBound().getWidth();
                setValue(mValueAtPress + percent * (mMaxValue - mMinValue));
            }
            else if (mDragOrientation == DragOrientation::Vertical)
            {
                setValue(mValueAtPress + (point.getY() - origin.getY()) / getBound().getHeight() * (mMaxValue - mMinValue));
            }
        }
        if (mDragType == DragType::Click)
        {
            if (mDragOrientation == DragOrientation::Horizontal)
            {
                double percent = (point.getX() - getLocalBound().getLeft()) / getBound().getWidth();
                setValue(percent * (mMaxValue - mMinValue) + mMinValue);
            }
            if (mDragOrientation == DragOrientation::Vertical)
            {
                setValue((point.getY() - getLocalBound().getTop()) / getBound().getHeight() * (mMaxValue - mMinValue) + mMinValue);
            }
        }
    }
    void AbstractSlider::setPercent(float percent)
    {
        mPercent = percent;
        updateVisualWithPercent(mPercent);
    }
    void AbstractSlider::updateValue()
    {
        setValue(mValue);
    }
    void AbstractSlider::updateVisualWithPercent(float percent)
    {
    }
    void AbstractSlider::onValueChanged(float value)
    {
    }
}