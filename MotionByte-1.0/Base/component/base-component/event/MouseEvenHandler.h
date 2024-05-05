#pragma once
#include <functional>
#include <iostream>
#include "util/geometry/Point.h"

namespace MotionByte
{
	class MouseEvenHandler
	{
	public:
		enum MouseButton
		{
			Left = 0,
			Right,
			Middle
		};
		enum MouseAction
		{
			Pressed = 0,
			Released
		};
	private:
		Point pressPoint;
		bool mIsPress = false;
		bool mIsInside = false;
		std::function<void(Point)> clickCallback;
		std::function<void(Point)> pressedCallback;
		std::function<void(Point)> releasedCallback;
		std::function<void(Point,double,double)> scrollCallback;
		std::function<void(Point)> clickOutsideCallback;
	protected:
		virtual void onClickedOutside(Point point)
		{

		}
		virtual void onScroll(Point point, double xValue, double yValue)
		{

		}
		virtual void onInput(Point point, MouseButton button, MouseAction mouseEvent)
		{

		}
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
		virtual void onMouseDragged(Point point, Point origin)
		{

		}
		virtual void onMouseEntered()
		{
		}
		virtual void onMouseExited()
		{

		}
		virtual void recursiveClickOutside(Point point)
		{

		}
		virtual void onMouseFocused()
		{

		}
		virtual void onMouseUnfocused()
		{

		}
		virtual void onFocused()
		{

		}
		virtual void onUnfocused()
		{

		}
		virtual void recursiveScroll(Point point, double xValue, double yValue) = 0;
		virtual void recursiveAction(Point point, bool& handled, MouseButton button, MouseAction mouseEvent) = 0;
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
			return mIsInside;
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
		void setScrollCallback(std::function<void(Point, double, double) > callback)
		{
			scrollCallback = callback;
		}
		void setClickOutsideCallback(std::function<void(Point)> callback)
		{
			clickOutsideCallback = callback;
		}
		void clickOutside(Point point)
		{
			bool handled = false;
			onClickedOutside(point);
			onUnfocused();
			if (clickOutsideCallback)
			{
				clickOutsideCallback(point);
			}
			recursivePress(point, handled);
			std::cout << "click outside at "
				<< point.getX().getValue() << " " << point.getY().getValue() << std::endl;
		}
		void clickAt(Point point)
		{
			bool handled = false;
			onClicked(point);
			onFocused();
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
				onMouseDragged(point,pressPoint);
			}
		}
		void releaseAt(Point point)
		{
			if (isPressing())
			{
				clickAt(point);
				if (isHoverOn() == false)
				{
					onMouseUnfocused();
				}
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
		void mouseAction(Point point, MouseButton button, MouseAction mouseEvent)
		{

			bool handled = false;
			recursiveAction(point, handled, button, mouseEvent);
			onInput(point, button, mouseEvent);
			handled = true;
		}
		void scrollAt(Point point, double xValue, double yValue)
		{
			recursiveScroll(point, xValue, yValue);
			if (scrollCallback)
			{
				scrollCallback(point, xValue, yValue);
			}
			onScroll(point, xValue, yValue);
			std::cout << "Scroll at "
				<< point.getX().getValue() << " " << point.getY().getValue() << std::endl;
			std::cout << "With value "
				<< xValue << " " << yValue << std::endl;
		}
		void mouseEnter()
		{
			if (isHoverOn() == false)
			{
				mIsInside = true;
				onMouseEntered();
				if (isPressing()==false)
					onMouseFocused();
				test_debug("Mouse enter");
			}
		}
		void mouseExit()
		{
			if (isHoverOn())
			{
				onMouseExited();
				if (isPressing()==false)
					onMouseUnfocused();
				test_debug("Mouse exit");
			}
			mIsInside = false;
		}
		Point getPressedPoint()
		{
			return pressPoint;
		}
	};
}
