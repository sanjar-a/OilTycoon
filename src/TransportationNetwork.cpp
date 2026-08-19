#include "TransportationNetwork.h"

#include <algorithm>

TransportationNetwork::TransportationNetwork(
    int networkId,
    int reservoirId
)
    : id(networkId),
      reservoirId(reservoirId),
      built(false),
      constructionDaysRemaining(0),
      baseCapacityPerDay(0.0),
      capacityPerDay(0.0),
      baseCostPerBarrel(0.0),
      costPerBarrel(0.0),
      baseRange(0.0),
      range(0.0),
      constructionCost(0.0),
      capacityUpgradeTier(0),
      costUpgradeTier(0),
      rangeUpgradeTier(0)
{
}

int TransportationNetwork::getId() const
{
    return id;
}

int TransportationNetwork::getReservoirId() const
{
    return reservoirId;
}

int TransportationNetwork::getCapacityUpgradeTier() const
{
    return capacityUpgradeTier;
}

int TransportationNetwork::getCostUpgradeTier() const
{
    return costUpgradeTier;
}

int TransportationNetwork::getRangeUpgradeTier() const
{
    return rangeUpgradeTier;
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

void TransportationNetwork::setCostPerBarrel(double cost)
{
    costPerBarrel = cost;
}

double TransportationNetwork::getRange() const
{
    return range;
}

void TransportationNetwork::setRange(double value)
{
    range = value;
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

    baseCapacityPerDay = capacity;
    capacityPerDay = capacity;

    baseCostPerBarrel = barrelCost;
    costPerBarrel = barrelCost;

    baseRange = transportationRange;
    range = transportationRange;

    capacityUpgradeTier = 0;
    costUpgradeTier = 0;
    rangeUpgradeTier = 0;
}

bool TransportationNetwork::upgradeCapacity(
    double multiplier,
    int tier
)
{
    if (!built || tier <= capacityUpgradeTier)
    {
        return false;
    }

    capacityPerDay =
        baseCapacityPerDay * multiplier;

    capacityUpgradeTier = tier;

    return true;
}

bool TransportationNetwork::upgradeCostPerBarrel(
    double multiplier,
    int tier
)
{
    if (!built || tier <= costUpgradeTier)
    {
        return false;
    }

    costPerBarrel =
        baseCostPerBarrel * multiplier;

    costUpgradeTier = tier;

    return true;
}

bool TransportationNetwork::upgradeRange(
    double additionalRange,
    int tier
)
{
    if (!built || tier <= rangeUpgradeTier)
    {
        return false;
    }

    range =
        baseRange + additionalRange;

    rangeUpgradeTier = tier;

    return true;
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

void TransportationNetwork::setCapacityPerDay(double capacity)
{
    capacityPerDay = capacity;
}
