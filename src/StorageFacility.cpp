#include "StorageFacility.h"

#include <algorithm>

StorageFacility::StorageFacility()
    : built(false),
      constructionDaysRemaining(0),
      capacity(0.0),
      inventory(0.0),
      costPerBarrel(0.0),
      constructionCost(0.0)
{
}

bool StorageFacility::isBuilt() const
{
    return built;
}

bool StorageFacility::isUnderConstruction() const
{
    return constructionDaysRemaining > 0;
}

int StorageFacility::getConstructionDaysRemaining() const
{
    return constructionDaysRemaining;
}

double StorageFacility::getCapacity() const
{
    return capacity;
}

double StorageFacility::getInventory() const
{
    return inventory;
}

double StorageFacility::getAvailableCapacity() const
{
    return capacity - inventory;
}

double StorageFacility::getCostPerBarrel() const
{
    return costPerBarrel;
}

double StorageFacility::getConstructionCost() const
{
    return constructionCost;
}

void StorageFacility::startConstruction(
    int constructionDays,
    double cost,
    double storageCapacity,
    double storageCostPerBarrel
)
{
    built = false;
    constructionDaysRemaining = constructionDays;

    constructionCost = cost;
    capacity = storageCapacity;
    costPerBarrel = storageCostPerBarrel;
}

bool StorageFacility::advanceConstruction()
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

double StorageFacility::addOil(double barrels)
{
    if (!built || barrels <= 0.0)
    {
        return 0.0;
    }

    const double accepted =
        std::min(barrels, getAvailableCapacity());

    inventory += accepted;

    return accepted;
}

double StorageFacility::removeOil(double barrels)
{
    if (!built || barrels <= 0.0)
    {
        return 0.0;
    }

    const double removed =
        std::min(barrels, inventory);

    inventory -= removed;

    return removed;
}

double StorageFacility::sellOil(double barrels)
{
    return removeOil(barrels);
}
