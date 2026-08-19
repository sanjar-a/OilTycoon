#ifndef OILTYCOON_TRANSPORTATIONNETWORK_H
#define OILTYCOON_TRANSPORTATIONNETWORK_H

class TransportationNetwork
{
private:
    int id;
    int reservoirId;
    bool built;
    int constructionDaysRemaining;

    double baseCapacityPerDay;
    double capacityPerDay;

    double baseCostPerBarrel;
    double costPerBarrel;

    double baseRange;
    double range;

    double constructionCost;

    int capacityUpgradeTier;
    int costUpgradeTier;
    int rangeUpgradeTier;

public:
    TransportationNetwork(
    int networkId,
    int reservoirId
    );

    int getId() const;
    int getReservoirId() const;
    bool isBuilt() const;
    bool isUnderConstruction() const;

    int getConstructionDaysRemaining() const;

    double getCapacityPerDay() const;
    void setCapacityPerDay(double capacity);
    double getCostPerBarrel() const;
    void setCostPerBarrel(double cost);
    double getRange() const;
    void setRange(double range);

    double getConstructionCost() const;

    int getCapacityUpgradeTier() const;
    int getCostUpgradeTier() const;
    int getRangeUpgradeTier() const;

    void startConstruction(
        int constructionDays,
        double constructionCost,
        double capacityPerDay,
        double costPerBarrel,
        double range
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

    bool upgradeRange(
        double additionalRange,
        int tier
    );

    double transport(double availableOil);
};

#endif