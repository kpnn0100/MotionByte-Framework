#include <component/base-component/VisualObject.h>

namespace MotionByte
{
    class Label : public VisualObject
    {
    private:
        std::string mText;
        Color mTextColor;
        Property mTextSize;
        Align mTextAlignment;
        void onParentChanged() override;
        void paint(Frame& frame) override;
    public:
        static const double DEFAULT_TEXT_SIZE;
        static const Color DEFAULT_TEXT_COLOR;
        static const Align DEFAULT_TEXT_ALIGNMENT; 
        static const double DEFAULT_WIDTH;
        static const double DEFAULT_HEIGHT; 
        Label();
        Label(std::string text);
        void setAlignment(Align alignment);
        void setText(std::string text);
        void setTextColor(Color color);
        void setTextSize(double size);
        std::string getText();
        Color & getTextColor();
        Property & getTextSize();
    };
} // namespace MotionByte
