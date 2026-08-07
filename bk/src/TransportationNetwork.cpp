#include "TransportationNetwork.h"

#include <algorithm>

TransportationNetwork::TransportationNetwork()
    : built(false),
      constructionDaysRemaining(0),
      capacityPerDay(0.0),
      costPerBarrel(0.0),
      range(0.0),
      constructionCost(0.0)
{
}

bool TransportationNetwork::isBuilt() const
{
    return built;
}

bool TransportationNetwork::isUnderConstruction() const
{
    return constructionDaysRemaining > 0;
}

int TransportationNetwork::getConstructionDaysRemaining() const
{
    return constructionDaysRemaining;
}

double TransportationNetwork::getCapacityPerDay() const
{
    return capacityPerDay;
}

double TransportationNetwork::getCostPerBarrel() const
{
    return costPerBarrel;
}

double TransportationNetwork::getRange() const
{
    return range;
}

double TransportationNetwork::getConstructionCost() const
{
    return constructionCost;
}

void TransportationNetwork::startConstruction(
    int constructionDays,
    double cost,
    double capacity,
    double barrelCost,
    double transportationRange
)
{
    built = false;
    constructionDaysRemaining = constructionDays;

    constructionCost = cost;
    capacityPerDay = capacity;
    costPerBarrel = barrelCost;
    range = transportationRange;
}

bool TransportationNetwork::advanceConstruction()
{
    if (built || constructionDaysRemaining <= 0)
    {
        return false;
    }

    --constructionDaysRemaining;

    if (constructionDaysRemaining <= 0)
    {
        constructionDaysRemaining = 0;
        built = true;

        return true;
    }

    return false;
}

double TransportationNetwork::transport(double availableOil)
{
    if (!built || availableOil <= 0.0)
    {
        return 0.0;
    }

    return std::min(availableOil, capacityPerDay);
}
