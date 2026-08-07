#include "Types.h"

std::string maintenanceStateToString(
    MaintenanceState state
)
{
    switch (state)
    {
    case MaintenanceState::Excellent:
        return "Excellent";

    case MaintenanceState::Good:
        return "Good";

    case MaintenanceState::Fair:
        return "Fair";

    case MaintenanceState::Poor:
        return "Poor";

    case MaintenanceState::Critical:
        return "Critical";

    case MaintenanceState::Failed:
        return "Failed";
    }

    return "Unknown";
}

std::string wellStatusToString(
    WellStatus status
)
{
    switch (status)
    {
    case WellStatus::Producing:
        return "Producing";

    case WellStatus::UnderMaintenance:
        return "Under Maintenance";

    case WellStatus::ShutDown:
        return "Shut Down";
    }

    return "Unknown";
}

std::string maintenancePolicyToString(
    MaintenancePolicy policy
)
{
    switch (policy)
    {
    case MaintenancePolicy::Manual:
        return "Manual";

    case MaintenancePolicy::AutoRepair:
        return "AutoRepair";

    case MaintenancePolicy::AutoRepairBelow40:
        return "AutoRepairBelow40";
    }

    return "Unknown";
}

std::string productionTechnologyToString(
    ProductionTechnology technology
)
{
    switch (technology)
    {
    case ProductionTechnology::NaturalFlow:
        return "Natural Flow";
    }

    return "Unknown";
}

double maintenanceMultiplier(
    MaintenanceState state
)
{
    switch (state)
    {
    case MaintenanceState::Excellent:
        return 1.0;

    case MaintenanceState::Good:
        return 0.8;

    case MaintenanceState::Fair:
        return 0.6;

    case MaintenanceState::Poor:
        return 0.4;

    case MaintenanceState::Critical:
        return 0.2;

    case MaintenanceState::Failed:
        return 0.0;
    }

    return 0.0;
}

int maintenancePercentage(
    MaintenanceState state
)
{
    switch (state)
    {
    case MaintenanceState::Excellent:
        return 100;

    case MaintenanceState::Good:
        return 80;

    case MaintenanceState::Fair:
        return 60;

    case MaintenanceState::Poor:
        return 40;

    case MaintenanceState::Critical:
        return 20;

    case MaintenanceState::Failed:
        return 0;
    }

    return 0;
}
