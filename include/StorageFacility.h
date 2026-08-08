#ifndef OILTYCOON_STORAGEFACILITY_H
#define OILTYCOON_STORAGEFACILITY_H

class StorageFacility
{
private:
    bool built;
    int constructionDaysRemaining;

    double capacity;
    double inventory;
    double costPerBarrel;

    double constructionCost;

public:
    StorageFacility();

    bool isBuilt() const;
    bool isUnderConstruction() const;

    int getConstructionDaysRemaining() const;

    double getCapacity() const;
    double getInventory() const;
    double getAvailableCapacity() const;

    double getCostPerBarrel() const;
    double getConstructionCost() const;

    void startConstruction(
        int constructionDays,
        double constructionCost,
        double capacity,
        double costPerBarrel
    );

    bool advanceConstruction();

    double addOil(double barrels);
    double removeOil(double barrels);

    double sellOil(double barrels);
};

#endif
