#pragma once
#include <vector>
#include "../Graphic/Color.h"
namespace MotionByte
{
    class ColorManager
    {
    private:
        std::vector<Color*> mColorList;
        int setCount = 0;
    public:
        ColorManager(int colorCount);
        void setInterpolatorForAll(std::shared_ptr<Interpolator> interpolator);
        void setInterpolator(int id, std::shared_ptr<Interpolator> interpolator);
        void initColor(int id, Color value);
        void setColor(int id, Color value);
        Color getColorValue(int id);
        Color getTargetColor(int id);
        Color& getColor(int id);
        void setColorPointer(int id, Color* color);
    };
}