#include "Reservoir.h"

#include <algorithm>

Reservoir::Reservoir(
    int id,
    const std::string& name,
    const Location& location,
    double initialReserves,
    double initialPressure,
    double depth
)
    : id(id),
      name(name),
      location(location),
      initialReserves(initialReserves),
      remainingReserves(initialReserves),
      initialPressure(initialPressure),
      currentPressure(initialPressure),
      depth(depth)
{
}

int Reservoir::getId() const
{
    return id;
}

const std::string& Reservoir::getName() const
{
    return name;
}

const Location& Reservoir::getLocation() const
{
    return location;
}

double Reservoir::getInitialReserves() const
{
    return initialReserves;
}

double Reservoir::getRemainingReserves() const
{
    return remainingReserves;
}

double Reservoir::getInitialPressure() const
{
    return initialPressure;
}

double Reservoir::getCurrentPressure() const
{
    return currentPressure;
}

double Reservoir::getDepth() const
{
    return depth;
}

void Reservoir::produce(double barrels)
{
    if (barrels <= 0.0 || remainingReserves <= 0.0)
    {
        return;
    }

    const double actualProduction =
        std::min(barrels, remainingReserves);

    remainingReserves -= actualProduction;

    // Simple transparent pressure model.
    // Pressure decreases proportionally to remaining reserves.
    if (initialReserves > 0.0)
    {
        const double reserveFraction =
            remainingReserves / initialReserves;

        currentPressure =
            initialPressure * reserveFraction;
    }

    if (remainingReserves <= 0.0)
    {
        remainingReserves = 0.0;
        currentPressure = 0.0;
    }
}

double Reservoir::getPressureFactor(
    double naturalFlowThreshold,
    double naturalFlowCutoff
) const
{
    if (currentPressure >= naturalFlowThreshold)
    {
        return 1.0;
    }

    if (currentPressure <= naturalFlowCutoff)
    {
        return 0.0;
    }

    return
        (currentPressure - naturalFlowCutoff) /
        (naturalFlowThreshold - naturalFlowCutoff);
}

bool Reservoir::isDepleted() const
{
    return remainingReserves <= 0.0;
}
