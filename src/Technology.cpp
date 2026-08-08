#include "Technology.h"

#include <algorithm>

TechnologyTree::TechnologyTree()
    : technologies()
{
    technologies = {
        {
            "exploration_cost_1",
            "Exploration Cost I",
            TechnologyCategory::Exploration,
            TechnologyMetric::Cost,
            1,
            10000.0,
            0.90,
            "",
            false
        },
        {
            "exploration_cost_2",
            "Exploration Cost II",
            TechnologyCategory::Exploration,
            TechnologyMetric::Cost,
            2,
            25000.0,
            0.80,
            "exploration_cost_1",
            false
        },
        {
            "exploration_time_1",
            "Exploration Time I",
            TechnologyCategory::Exploration,
            TechnologyMetric::Time,
            1,
            10000.0,
            0.80,
            "",
            false
        },
        {
            "exploration_accuracy_1",
            "Exploration Accuracy I",
            TechnologyCategory::Exploration,
            TechnologyMetric::Accuracy,
            1,
            15000.0,
            0.15,
            "",
            false
        },

        {
            "drilling_cost_1",
            "Drilling Cost I",
            TechnologyCategory::Drilling,
            TechnologyMetric::Cost,
            1,
            20000.0,
            0.90,
            "",
            false
        },
        {
            "drilling_time_1",
            "Drilling Time I",
            TechnologyCategory::Drilling,
            TechnologyMetric::Time,
            1,
            20000.0,
            0.80,
            "",
            false
        },
        {
            "drilling_capability_1",
            "Well Capability I",
            TechnologyCategory::Drilling,
            TechnologyMetric::WellCapability,
            1,
            25000.0,
            1.20,
            "",
            false
        },
        {
            "drilling_depth_1",
            "Maximum Depth I",
            TechnologyCategory::Drilling,
            TechnologyMetric::MaximumDepth,
            1,
            30000.0,
            3500.0,
            "",
            false
        },
        {
            "drilling_depth_2",
            "Maximum Depth II",
            TechnologyCategory::Drilling,
            TechnologyMetric::MaximumDepth,
            2,
            60000.0,
            4000.0,
            "drilling_depth_1",
            false
        },

        {
            "maintenance_degradation_1",
            "Maintenance Reliability I",
            TechnologyCategory::Maintenance,
            TechnologyMetric::DegradationRate,
            1,
            20000.0,
            0.80,
            "",
            false
        },
        {
            "maintenance_repair_1",
            "Repair Time I",
            TechnologyCategory::Maintenance,
            TechnologyMetric::RepairTime,
            1,
            20000.0,
            0.80,
            "",
            false
        },
        {
            "maintenance_parts_1",
            "Spare Parts I",
            TechnologyCategory::Maintenance,
            TechnologyMetric::SparePartsConsumption,
            1,
            20000.0,
            0.80,
            "",
            false
        },

        {
            "transport_capacity_1",
            "Transportation Capacity I",
            TechnologyCategory::Transportation,
            TechnologyMetric::Capacity,
            1,
            25000.0,
            1.25,
            "",
            false
        },
        {
            "transport_cost_1",
            "Transportation Cost I",
            TechnologyCategory::Transportation,
            TechnologyMetric::CostPerBarrel,
            1,
            20000.0,
            0.85,
            "",
            false
        },
        {
            "transport_range_1",
            "Transportation Range I",
            TechnologyCategory::Transportation,
            TechnologyMetric::Range,
            1,
            25000.0,
            150.0,
            "",
            false
        },

        {
            "storage_capacity_1",
            "Storage Capacity I",
            TechnologyCategory::Storage,
            TechnologyMetric::Capacity,
            1,
            20000.0,
            1.50,
            "",
            false
        },
        {
            "storage_cost_1",
            "Storage Cost I",
            TechnologyCategory::Storage,
            TechnologyMetric::CostPerBarrel,
            1,
            15000.0,
            0.85,
            "",
            false
        }
    };
}

const std::vector<Technology>& TechnologyTree::getTechnologies() const
{
    return technologies;
}

bool TechnologyTree::unlockTechnology(
    const std::string& technologyId,
    double& companyMoney
)
{
    for (Technology& technology : technologies)
    {
        if (technology.id != technologyId)
        {
            continue;
        }

        if (technology.unlocked)
        {
            return false;
        }

        if (!technology.prerequisiteId.empty() &&
            !isUnlocked(technology.prerequisiteId))
        {
            return false;
        }

        if (companyMoney < technology.cost)
        {
            return false;
        }

        companyMoney -= technology.cost;
        technology.unlocked = true;

        return true;
    }

    return false;
}

bool TechnologyTree::isUnlocked(
    const std::string& technologyId
) const
{
    for (const Technology& technology : technologies)
    {
        if (technology.id == technologyId)
        {
            return technology.unlocked;
        }
    }

    return false;
}

double TechnologyTree::getEffect(
    TechnologyCategory category,
    TechnologyMetric metric
) const
{
    double result = 1.0;

    for (const Technology& technology : technologies)
    {
        if (technology.category != category ||
            technology.metric != metric ||
            !technology.unlocked)
        {
            continue;
        }

        if (metric == TechnologyMetric::MaximumDepth)
        {
            result = std::max(result, technology.effect);
        }
        else if (metric == TechnologyMetric::Accuracy)
        {
            result += technology.effect;
        }
        else if (metric == TechnologyMetric::Range)
        {
            result += technology.effect;
        }
        else
        {
            result *= technology.effect;
        }
    }

    return result;
}

int TechnologyTree::getHighestTier(
    TechnologyCategory category,
    TechnologyMetric metric
) const
{
    int highestTier = 0;

    for (const Technology& technology : technologies)
    {
        if (technology.category == category &&
            technology.metric == metric &&
            technology.unlocked)
        {
            highestTier =
                std::max(highestTier, technology.tier);
        }
    }

    return highestTier;
}
