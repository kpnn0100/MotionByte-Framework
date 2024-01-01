#pragma once
#include <functional>
#include <iostream>
#include "../Point.h"

namespace pertyG
{
	class MouseEvenHandler
	{
	private:
		Point pressPoint;
		bool mIsPress = false;
		bool mIsInside = false;
		std::function<void(Point)> clickCallback;
		std::function<void(Point)> pressedCallback;
		std::function<void(Point)> releasedCallback;


	protected:
		virtual void onClicked(Point point)
		{
			// Your implementation for click event
		}

		virtual void onPressed(Point point)
		{
			// Your implementation for press event

		}

		virtual void onReleased(Point point)
		{
			// Your implementation for release event
		}
		virtual void onMouseDragged(Point point)
		{

		}
		virtual void onMouseEntered()
		{
		}
		virtual void onMouseExited()
		{

		}
		virtual void recursivePress(Point point, bool& handled) = 0;
		virtual void recursiveRelease(Point point, bool& handled) = 0;
		virtual void recursiveMove(Point point) = 0;

	public:
		bool isPressing()
		{
			return mIsPress;
		}
		bool isHoverOn()
		{
			return mIsPress;
		}
		void setClickCallback(std::function<void(Point)> callback)
		{
			clickCallback = callback;
		}

		void setPressedCallback(std::function<void(Point)> callback)
		{
			pressedCallback = callback;
		}

		void setReleasedCallback(std::function<void(Point)> callback)
		{
			releasedCallback = callback;
		}
		void clickAt(Point point)
		{
			bool handled = false;
			onClicked(point);

			if (clickCallback)
			{
				clickCallback(point);
			}

			std::cout << "Clicked at "
				<< point.getX().getValue() << " " << point.getY().getValue() << std::endl;
		}
		void pressAt(Point point)
		{

			mIsPress = true;
			pressPoint = point;
			onPressed(point);
			bool handled = false;
			recursivePress(point, handled);
			if (pressedCallback)
			{
				pressedCallback(point);
			}
			std::cout << "Pressed at "
				<< point.getX().getValue() << " " << point.getY().getValue() << std::endl;
		}
		void mouseMove(Point point)
		{
			recursiveMove(point);
			if (isPressing())
			{
				onMouseDragged(point);
			}
		}
		void releaseAt(Point point)
		{
			if (isPressing())
			{
				clickAt(point);
			}
			mIsPress = false;
			pressPoint = Point();
			onReleased(point);
			bool handled = false;
			recursiveRelease(point, handled);
			if (releasedCallback)
			{
				releasedCallback(point);
			}
			std::cout << "Released at "
				<< point.getX().getValue() << " " << point.getY().getValue() << std::endl;
		}
		void mouseEnter()
		{
			mIsInside = true;
			onMouseEntered();
		}
		void mouseExit()
		{
			if (isHoverOn())
			{
				onMouseExited();
			}
			mIsInside = false;
		}
		Point getPressedPoint()
		{
			return pressPoint;
		}
	};
}
