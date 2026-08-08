#ifndef OILTYCOON_DRILLINGPROJECT_H
#define OILTYCOON_DRILLINGPROJECT_H

#include "Types.h"

class Reservoir;

class DrillingProject
{
private:
    ProjectStatus status;

    Reservoir* reservoir;

    int daysRemaining;

    double cost;
    double wellCapability;

public:
    DrillingProject();

    bool isActive() const;
    bool isCompleted() const;

    ProjectStatus getStatus() const;

    Reservoir* getReservoir();

    int getDaysRemaining() const;
    double getCost() const;
    double getWellCapability() const;

    void start(
        Reservoir* reservoir,
        int durationDays,
        double cost,
        double wellCapability
    );

    bool advance();

    void reset();
};

#endif
