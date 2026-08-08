#ifndef OILTYCOON_WELL_H
#define OILTYCOON_WELL_H

#include "Types.h"

class Reservoir;

class Well
{
private:
    int id;
    Reservoir* reservoir;

    double maxProductionRate;

    double naturalFlowThresholdPressure;
    double naturalFlowCutoffPressure;

    ProductionTechnology productionTechnology;
    MaintenanceState maintenanceState;
    WellStatus status;

    int repairDaysRemaining;

public:
    Well(
        int id,
        Reservoir* reservoir,
        double maxProductionRate,
        double naturalFlowThresholdPressure,
        double naturalFlowCutoffPressure
    );

    int getId() const;

    const Reservoir* getReservoir() const;
    Reservoir* getReservoir();

    double getMaxProductionRate() const;
    double getNaturalFlowThresholdPressure() const;
    double getNaturalFlowCutoffPressure() const;

    ProductionTechnology getProductionTechnology() const;
    MaintenanceState getMaintenanceState() const;
    WellStatus getStatus() const;

    int getRepairDaysRemaining() const;

    double calculateProduction() const;

    void degrade();

    bool startRepair(int repairDays);

    bool processRepair();

    void setMaintenanceState(MaintenanceState state);

    bool needsAutoRepair() const;
};

#endif
