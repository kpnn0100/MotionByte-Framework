#pragma once
#include <functional>
#include <iostream>
#include "../Point.h"
namespace pertyG
{
	class MouseEvenHandler
	{
	private:
		std::function<void(Point)> clickCallback;
		virtual void onClicked(Point point)
		{

		}
	protected:
		virtual void recursiveClick(Point point, bool & handled) = 0;
	public:
		void clickAt(Point point)
		{

			bool handled = false;
			recursiveClick(point,handled);
			onClicked(point);
			if (clickCallback)
			{
				clickCallback(point);
			}
			std::cout  << " click at "
				<< point.getX().getValue() << " " << point.getY().getValue() << std::endl;
		}
		void setClickCallback(std::function<void(Point)> callback)
		{
			clickCallback = callback;
		}
	};
}