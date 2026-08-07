#include "Well.h"

#include "Reservoir.h"

Well::Well(
    int id,
    Reservoir* reservoir,
    double maxProductionRate,
    double naturalFlowThresholdPressure,
    double naturalFlowCutoffPressure
)
    : id(id),
      reservoir(reservoir),
      maxProductionRate(maxProductionRate),
      naturalFlowThresholdPressure(naturalFlowThresholdPressure),
      naturalFlowCutoffPressure(naturalFlowCutoffPressure),
      productionTechnology(ProductionTechnology::NaturalFlow),
      maintenanceState(MaintenanceState::Excellent),
      status(WellStatus::Producing),
      repairDaysRemaining(0)
{
}

int Well::getId() const
{
    return id;
}

const Reservoir* Well::getReservoir() const
{
    return reservoir;
}

Reservoir* Well::getReservoir()
{
    return reservoir;
}

double Well::getMaxProductionRate() const
{
    return maxProductionRate;
}

double Well::getNaturalFlowThresholdPressure() const
{
    return naturalFlowThresholdPressure;
}

double Well::getNaturalFlowCutoffPressure() const
{
    return naturalFlowCutoffPressure;
}

ProductionTechnology Well::getProductionTechnology() const
{
    return productionTechnology;
}

MaintenanceState Well::getMaintenanceState() const
{
    return maintenanceState;
}

WellStatus Well::getStatus() const
{
    return status;
}

int Well::getRepairDaysRemaining() const
{
    return repairDaysRemaining;
}

double Well::calculateProduction() const
{
    if (reservoir == nullptr)
    {
        return 0.0;
    }

    if (status != WellStatus::Producing)
    {
        return 0.0;
    }

    if (reservoir->isDepleted())
    {
        return 0.0;
    }

    const double pressureFactor =
        reservoir->getPressureFactor(
            naturalFlowThresholdPressure,
            naturalFlowCutoffPressure
        );

    double technologyFactor = 1.0;

    if (productionTechnology == ProductionTechnology::NaturalFlow)
    {
        technologyFactor = 1.0;
    }

    const double maintenanceFactor =
        maintenanceMultiplier(maintenanceState);

    return
        maxProductionRate *
        pressureFactor *
        technologyFactor *
        maintenanceFactor;
}

void Well::degrade()
{
    if (status != WellStatus::Producing)
    {
        return;
    }

    switch (maintenanceState)
    {
    case MaintenanceState::Excellent:
        maintenanceState = MaintenanceState::Good;
        break;

    case MaintenanceState::Good:
        maintenanceState = MaintenanceState::Fair;
        break;

    case MaintenanceState::Fair:
        maintenanceState = MaintenanceState::Poor;
        break;

    case MaintenanceState::Poor:
        maintenanceState = MaintenanceState::Critical;
        break;

    case MaintenanceState::Critical:
        maintenanceState = MaintenanceState::Failed;
        break;

    case MaintenanceState::Failed:
        break;
    }
}

bool Well::startRepair(int repairDays)
{
    if (status == WellStatus::UnderMaintenance)
    {
        return false;
    }

    if (maintenanceState == MaintenanceState::Excellent)
    {
        return false;
    }

    if (repairDays <= 0)
    {
        return false;
    }

    status = WellStatus::UnderMaintenance;
    repairDaysRemaining = repairDays;

    return true;
}

bool Well::processRepair()
{
    if (status != WellStatus::UnderMaintenance)
    {
        return false;
    }

    --repairDaysRemaining;

    if (repairDaysRemaining <= 0)
    {
        repairDaysRemaining = 0;
        maintenanceState = MaintenanceState::Excellent;
        status = WellStatus::Producing;

        return true;
    }

    return false;
}

void Well::setMaintenanceState(MaintenanceState state)
{
    maintenanceState = state;

    if (state == MaintenanceState::Failed)
    {
        status = WellStatus::ShutDown;
    }
}

bool Well::needsAutoRepair() const
{
    return
        status == WellStatus::Producing &&
        maintenanceState != MaintenanceState::Excellent;
}
