#pragma once
#include <vector>
#include "util/geometry/Color.h"
namespace MotionByte
{
    class ColorManager
    {
    private:
        std::vector<Color*> mColorList;
        int setCount = 0;
    public:
        ColorManager(int colorCount);
        void setInterpolatorForAll(InterpolatorModule InterpolatorModule);
        void setInterpolator(int id, InterpolatorModule InterpolatorModule);
        void initColor(int id, Color value);
        void setColor(int id, Color value);
        Color getColorValue(int id);
        Color getTargetColor(int id);
        Color& getColor(int id);
        void setColorPointer(int id, Color* color);
    };
}