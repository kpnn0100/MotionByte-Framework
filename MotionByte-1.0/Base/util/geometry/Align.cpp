#include "Align.h"
namespace MotionByte
{
	const Align Align::TopLeft(Align::Left, Align::Top);
	const Align Align::TopMiddle(Align::Middle, Align::Top);
	const Align Align::TopRight(Align::Right, Align::Top);
	const Align Align::CenterLeft(Align::Left, Align::Center);
	const Align Align::CenterMiddle(Align::Middle, Align::Center);
	const Align Align::CenterRight(Align::Right, Align::Center);
	const Align Align::BottomLeft(Align::Left, Align::Bottom);
	const Align Align::BottomMiddle(Align::Middle, Align::Bottom);
	const Align Align::BottomRight(Align::Right, Align::Bottom);
	Align::Align()
	{
		mHorizontal = Align::Middle;
		mVertical = Align::Center;
	}
    Align::Align(Align const & align)
    {
		mHorizontal = align.mHorizontal;
		mVertical = align.mVertical;
    }
}