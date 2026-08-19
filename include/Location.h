#ifndef OILTYCOON_LOCATION_H
#define OILTYCOON_LOCATION_H

class Location
{
private:
    double x;
    double y;

public:
    Location();
    Location(double x, double y);

    double getX() const;
    double getY() const;

    double distanceTo(const Location& other) const;
};

#endif