#ifndef OILTYCOON_TECHNOLOGY_H
#define OILTYCOON_TECHNOLOGY_H

#include <string>
#include <vector>

enum class TechnologyDomain
{
    Operational,
    Infrastructure
};

enum class TechnologyCategory
{
    Exploration,
    Drilling,
    Maintenance,
    Transportation,
    Storage
};

enum class TechnologyMetric
{
    Cost,
    Time,
    Accuracy,
    WellCapability,
    MaximumDepth,
    DegradationRate,
    RepairTime,
    SparePartsConsumption,
    Capacity,
    CostPerBarrel,
    Range
};

struct Technology
{
    std::string id;
    std::string name;

    TechnologyDomain domain;
    TechnologyCategory category;
    TechnologyMetric metric;

    int tier;

    double cost;
    double effect;

    std::string prerequisiteId;

    bool unlocked;
};

class TechnologyTree
{
private:
    std::vector<Technology> technologies;

public:
    TechnologyTree();

    const std::vector<Technology>& getTechnologies() const;

    bool unlockTechnology(
        const std::string& technologyId,
        double& companyMoney
    );

    bool isUnlocked(
        const std::string& technologyId
    ) const;

    double getEffect(
        TechnologyCategory category,
        TechnologyMetric metric
    ) const;

    double getEffectAtTier(
        TechnologyCategory category,
        TechnologyMetric metric,
        int tier
    ) const;

    bool isInfrastructureTechnologyUnlocked(
        TechnologyCategory category,
        TechnologyMetric metric,
        int tier
    ) const;

    int getHighestTier(
        TechnologyCategory category,
        TechnologyMetric metric
    ) const;
};

#endif