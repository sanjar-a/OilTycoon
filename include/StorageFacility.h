#ifndef OILTYCOON_STORAGEFACILITY_H
#define OILTYCOON_STORAGEFACILITY_H
#include "Location.h"

class StorageFacility
{
private:
    int id;
    Location location;

    bool built;
    int constructionDaysRemaining;

    double baseCapacity;
    double capacity;

    double inventory;
    double costPerBarrel;

    double constructionCost;

    int capacityUpgradeTier;
    int costUpgradeTier;

public:
    StorageFacility(
        int facilityId,
        const Location & location
    );

    int getId() const;
    const Location& getLocation() const;

    bool isBuilt() const;
    bool isUnderConstruction() const;

    int getConstructionDaysRemaining() const;

    double getCapacity() const;
    double getInventory() const;
    double getAvailableCapacity() const;

    double getCostPerBarrel() const;
    double getConstructionCost() const;

    int getCapacityUpgradeTier() const;
    int getCostUpgradeTier() const;

    void startConstruction(
        int constructionDays,
        double constructionCost,
        double capacity,
        double costPerBarrel
    );

    bool advanceConstruction();

    bool upgradeCapacity(
        double multiplier,
        int tier
    );

    bool upgradeCostPerBarrel(
        double multiplier,
        int tier
    );

    double addOil(double barrels);
    double removeOil(double barrels);

    double sellOil(double barrels);

    
    void setCostPerBarrel(double cost);
};

#endif