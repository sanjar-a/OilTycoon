#include "Location.h"

#include <cmath>

Location::Location()
    : x(0.0),
      y(0.0)
{
}

Location::Location(double x, double y)
    : x(x),
      y(y)
{
}

double Location::getX() const
{
    return x;
}

double Location::getY() const
{
    return y;
}

double Location::distanceTo(const Location& other) const
{
    const double deltaX = x - other.x;
    const double deltaY = y - other.y;

    return std::sqrt(
        deltaX * deltaX +
        deltaY * deltaY
    );
}