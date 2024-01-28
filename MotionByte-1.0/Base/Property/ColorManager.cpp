#include "ColorManager.h"
namespace MotionByte
{
    ColorManager::ColorManager(int colorCount) : mColorList(colorCount)
    {
    }
    void ColorManager::setInterpolatorForAll(std::shared_ptr<Interpolator> interpolator)
    {
        for (auto& color : mColorList)
        {
            color->setInterpolator(interpolator);
        }
    }
    void ColorManager::setInterpolator(int id, std::shared_ptr<Interpolator> interpolator)
    {
        mColorList[id]->setInterpolator(interpolator);
    }
    void ColorManager::initColor(int id, Color value)
    {
        mColorList[id]->initColor(value);
    }
    void ColorManager::setColor(int id, Color value)
    {

        mColorList[id]->setColor(value);
    }
    Color ColorManager::getColorValue(int id)
    {
        return mColorList[id]->getColor();
    }
    Color ColorManager::getTargetColor(int id)
    {
        return mColorList[id]->getTargetColor();
    }
    Color& ColorManager::getColor(int id)
    {
        return *mColorList[id];
    }
    void ColorManager::setColorPointer(int id, Color* color)
    {
        mColorList[id] = color;
    }
}