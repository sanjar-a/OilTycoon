#ifndef OILTYCOON_TRANSPORTATIONNETWORK_H
#define OILTYCOON_TRANSPORTATIONNETWORK_H

class TransportationNetwork
{
private:
    bool built;
    int constructionDaysRemaining;

    double capacityPerDay;
    double costPerBarrel;
    double range;

    double constructionCost;

public:
    TransportationNetwork();

    bool isBuilt() const;
    bool isUnderConstruction() const;

    int getConstructionDaysRemaining() const;

    double getCapacityPerDay() const;
    double getCostPerBarrel() const;
    double getRange() const;
    double getConstructionCost() const;

    void startConstruction(
        int constructionDays,
        double constructionCost,
        double capacityPerDay,
        double costPerBarrel,
        double range
    );

    bool advanceConstruction();

    double transport(double availableOil);
};

#endif
