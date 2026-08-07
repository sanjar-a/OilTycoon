#ifndef OILTYCOON_EXPLORATIONPROJECT_H
#define OILTYCOON_EXPLORATIONPROJECT_H

#include "Types.h"

class ExplorationProject
{
private:
    ProjectStatus status;
    ExplorationResult result;

    int daysRemaining;

    double cost;
    double accuracy;

public:
    ExplorationProject();

    bool isActive() const;
    bool isCompleted() const;

    ProjectStatus getStatus() const;
    ExplorationResult getResult() const;

    int getDaysRemaining() const;
    double getCost() const;
    double getAccuracy() const;

    void start(
        int durationDays,
        double cost,
        double accuracy
    );

    bool advance(double randomRoll);

    void reset();
};

#endif
