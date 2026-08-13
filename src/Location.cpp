#include "Location.h"

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