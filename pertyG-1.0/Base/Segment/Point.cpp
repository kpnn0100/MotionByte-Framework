#include "Point.h"
namespace pertyG
{
	Point::Point() : mPosition{ 0.0, 0.0 }
	{
	}
	Point::Point(double x, double y) : mPosition{ x, y }
	{
	}
	Point::Point(const Point& other)
	{
		this->mPosition[0] = other.mPosition[0];
		this->mPosition[1] = other.mPosition[1];
	}
	Point Point::operator=(const Point& other)
	{
		mPosition[0] = other.mPosition[0];
		mPosition[1] = other.mPosition[1];
		return *this;
	}
	Property &Point::getX() { 
		return mPosition[X]; 
	}
	Property &Point::getY() {
		return mPosition[Y];
	}
	Property &Point::getPosition(int dimension)
	{
		return mPosition[dimension];
	}
	void Point::setPosition(int dimension, double value) {
		if (dimension >= 0 && dimension < 2) {
			mPosition[dimension].setValue(value);
		}
	}

	void Point::move(Point vector)
	{
		for (int i = 0 ; i <2;i++)
			mPosition[i] = mPosition[i].shift(vector.mPosition[i].getTargetValue());
	}

	Point Point::withOffset(Point offset)
	{
		Point newPoint = *this;
		newPoint.setPosition(0, this->getX().getValue() + offset.getX().getValue());
		newPoint.setPosition(1, this->getY().getValue() + offset.getY().getValue());
		return newPoint;
	}

}