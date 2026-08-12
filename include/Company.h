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

    std::vector<std::unique_ptr<TransportationNetwork>>
        transportationNetworks;

    std::vector<std::unique_ptr<StorageFacility>>
        storageFacilities;

    TechnologyTree technologies;

    ExplorationProject explorationProject;
    DrillingProject drillingProject;

    bool hasCommercialDiscovery;

    int nextReservoirId;
    int nextWellId;
    int nextTransportationId;
    int nextStorageId;

public:
    Company();

    double getMoney() const;
    int getSpareParts() const;

    MaintenancePolicy getMaintenancePolicy() const;

    const std::vector<std::unique_ptr<Reservoir>>&
    getReservoirs() const;

    const std::vector<std::unique_ptr<Well>>&
    getWells() const;

    const std::vector<
        std::unique_ptr<TransportationNetwork>
    >& getTransportationNetworks() const;

    const std::vector<
        std::unique_ptr<StorageFacility>
    >& getStorageFacilities() const;

    TechnologyTree& getTechnologyTree();
    const TechnologyTree& getTechnologyTree() const;

    ExplorationProject& getExplorationProject();
    const ExplorationProject& getExplorationProject() const;

    DrillingProject& getDrillingProject();
    const DrillingProject& getDrillingProject() const;

    bool upgradeStorageCapacity(int storageId);
    bool upgradeStorageCost(int storageId);

    bool upgradeTransportationCapacity(
        int transportationId
    );

    bool upgradeTransportationCost(
        int transportationId
    );

    bool upgradeTransportationRange(
        int transportationId
    );
    
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

    void processStorageCosts();

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
