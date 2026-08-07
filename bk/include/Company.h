#ifndef OILTYCOON_COMPANY_H
#define OILTYCOON_COMPANY_H

#include "DrillingProject.h"
#include "ExplorationProject.h"
#include "Reservoir.h"
#include "StorageFacility.h"
#include "Technology.h"
#include "TransportationNetwork.h"
#include "Types.h"
#include "Well.h"

#include <memory>
#include <vector>

class Company
{
private:
    double money;
    int spareParts;

    MaintenancePolicy maintenancePolicy;

    std::vector<std::unique_ptr<Reservoir>> reservoirs;
    std::vector<std::unique_ptr<Well>> wells;

    TransportationNetwork transportation;
    StorageFacility storage;

    TechnologyTree technologies;

    ExplorationProject explorationProject;
    DrillingProject drillingProject;

    bool hasCommercialDiscovery;

    int nextReservoirId;
    int nextWellId;

public:
    Company();

    double getMoney() const;
    int getSpareParts() const;

    MaintenancePolicy getMaintenancePolicy() const;

    const std::vector<std::unique_ptr<Reservoir>>&
    getReservoirs() const;

    const std::vector<std::unique_ptr<Well>>&
    getWells() const;

    TransportationNetwork& getTransportation();
    const TransportationNetwork& getTransportation() const;

    StorageFacility& getStorage();
    const StorageFacility& getStorage() const;

    TechnologyTree& getTechnologyTree();
    const TechnologyTree& getTechnologyTree() const;

    ExplorationProject& getExplorationProject();
    const ExplorationProject& getExplorationProject() const;

    DrillingProject& getDrillingProject();
    const DrillingProject& getDrillingProject() const;

    bool startExploration();

    void advanceExploration(double randomRoll);

    bool startDrilling();

    void advanceDrilling();

    bool buildTransportation();

    bool buildStorage();

    void advanceConstruction();

    void processMaintenance();

    void processRepairs();

    double processProduction();

    double processTransportation(double producedOil);

    double processStorage(double transportedOil);

    bool repairWell(int wellId);

    bool setMaintenancePolicy(MaintenancePolicy policy);

    bool sellOil(double barrels, double oilPrice);

    bool buySpareParts(int amount, double pricePerPart);

    bool unlockTechnology(const std::string& technologyId);

    bool hasDiscovery() const;

    void printWells() const;
    void printReservoirs() const;
    void printTechnologies() const;
};

#endif
