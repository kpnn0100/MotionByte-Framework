/*
    MotionByte Framework
    Created by: kpnn0100 / Doan Le Nhat Nam
    Version: 1.0
*/

#pragma once
#include "Segment.h"

namespace MotionByte
{

    class VisualObject : public Segment
    {
    private:
        class InsiderObject : public Segment
        {
        private:
            friend class VisualObject;
            Segment* parentObject;
            Color mBorderColor, mBackgroundColor;
            Property mRadius, mBorderThickness;
            void onParentChanged() override;
            void paint(Frame& frame) override;
        public:
            InsiderObject();
        };
        PropertyManager mObjectPropertyList;
        ColorManager mObjectColorList;
        std::shared_ptr<InsiderObject> mInsiderObject = std::make_shared<InsiderObject>();
    public:
        VisualObject();
        PropertyManager& getObjectProperties();
        ColorManager& getObjectColors();
        void paint(Frame& frame) override;
        void setBorderThickness(double thickness);
        void setBorderColor(Color color);
        void setRadius(double radius);
        void setBackgroundColor(Color color);
        enum ObjectPropertyID {
            X,               
            Y,               
            Width,           
            Height,
            Radius,
            BorderThickness,
            Opacity,
            PropertyCount
        };
        enum ObjectColorID {
            Background,
            Border,
            ColorCount
        };
    };
}
