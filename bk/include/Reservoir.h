#ifndef OILTYCOON_RESERVOIR_H
#define OILTYCOON_RESERVOIR_H

#include <string>

class Reservoir
{
private:
    int id;
    std::string name;

    double initialReserves;
    double remainingReserves;

    double initialPressure;
    double currentPressure;

    double depth;

public:
    Reservoir(
        int id,
        const std::string& name,
        double initialReserves,
        double initialPressure,
        double depth
    );

    int getId() const;
    const std::string& getName() const;

    double getInitialReserves() const;
    double getRemainingReserves() const;

    double getInitialPressure() const;
    double getCurrentPressure() const;

    double getDepth() const;

    void produce(double barrels);

    double getPressureFactor(
        double naturalFlowThreshold,
        double naturalFlowCutoff
    ) const;

    bool isDepleted() const;
};

#endif
