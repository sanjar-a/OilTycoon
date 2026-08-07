#ifndef OILTYCOON_TYPES_H
#define OILTYCOON_TYPES_H

#include <string>

enum class MaintenanceState
{
    Excellent,
    Good,
    Fair,
    Poor,
    Critical,
    Failed
};

enum class WellStatus
{
    Producing,
    UnderMaintenance,
    ShutDown
};

enum class MaintenancePolicy
{
    Manual,
    AutoRepair,
    AutoRepairBelow40
};

enum class ProductionTechnology
{
    NaturalFlow
};

enum class ProjectStatus
{
    NotStarted,
    InProgress,
    Completed
};

enum class ExplorationResult
{
    None,
    CommercialDiscovery,
    DryHole
};

std::string maintenanceStateToString(MaintenanceState state);
std::string wellStatusToString(WellStatus status);
std::string maintenancePolicyToString(MaintenancePolicy policy);
std::string productionTechnologyToString(ProductionTechnology technology);

double maintenanceMultiplier(MaintenanceState state);
int maintenancePercentage(MaintenanceState state);

#endif
