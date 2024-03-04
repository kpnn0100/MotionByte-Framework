#pragma once
namespace MotionByte
{
	class Align
	{
	public:
		enum Horizontal {
			Left,
			Middle,
			Right
		};
		enum Vertical {
			Top,
			Center,
			Bottom
		};
		Horizontal mHorizontal;
		Vertical mVertical;
		Align(Horizontal h, Vertical v) : mHorizontal(h), mVertical(v)
		{

		}
		Horizontal getHorizontal()
		{
			return mHorizontal;
		}
		Vertical getVertical()
		{
			return mVertical;
		}
		// Static instances with assigned values
		static const Align TopLeft;
		static const Align TopMiddle;
		static const Align TopRight;
		static const Align CenterLeft;
		static const Align CenterMiddle;
		static const Align CenterRight;
		static const Align BottomLeft;
		static const Align BottomMiddle;
		static const Align BottomRight;
	};
}