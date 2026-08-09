#include "Company.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

Company::Company()
    : money(250000.0),
      spareParts(100),
      maintenancePolicy(MaintenancePolicy::Manual),
      reservoirs(),
      wells(),
      transportation(),
      storage(),
      technologies(),
      explorationProject(),
      drillingProject(),
      hasCommercialDiscovery(false),
      nextReservoirId(1),
      nextWellId(1)
{
}

double Company::getMoney() const
{
    return money;
}

int Company::getSpareParts() const
{
    return spareParts;
}

MaintenancePolicy Company::getMaintenancePolicy() const
{
    return maintenancePolicy;
}

const std::vector<std::unique_ptr<Reservoir>>&
Company::getReservoirs() const
{
    return reservoirs;
}

const std::vector<std::unique_ptr<Well>>&
Company::getWells() const
{
    return wells;
}

TransportationNetwork& Company::getTransportation()
{
    return transportation;
}

const TransportationNetwork& Company::getTransportation() const
{
    return transportation;
}

StorageFacility& Company::getStorage()
{
    return storage;
}

const StorageFacility& Company::getStorage() const
{
    return storage;
}

TechnologyTree& Company::getTechnologyTree()
{
    return technologies;
}

const TechnologyTree& Company::getTechnologyTree() const
{
    return technologies;
}

ExplorationProject& Company::getExplorationProject()
{
    return explorationProject;
}

const ExplorationProject&
Company::getExplorationProject() const
{
    return explorationProject;
}

DrillingProject& Company::getDrillingProject()
{
    return drillingProject;
}

const DrillingProject&
Company::getDrillingProject() const
{
    return drillingProject;
}

bool Company::startExploration()
{
    if (explorationProject.isActive())
    {
        return false;
    }

    if (hasCommercialDiscovery)
    {
        return false;
    }

    const double costMultiplier =
        technologies.getEffect(
            TechnologyCategory::Exploration,
            TechnologyMetric::Cost
        );

    const double timeMultiplier =
        technologies.getEffect(
            TechnologyCategory::Exploration,
            TechnologyMetric::Time
        );

    const double accuracyBonus =
        technologies.getEffect(
            TechnologyCategory::Exploration,
            TechnologyMetric::Accuracy
        ) - 1.0;

    const double baseCost = 30000.0;
    const int baseTime = 5;
    const double baseAccuracy = 0.60;

    const double cost =
        baseCost * costMultiplier;

    const int duration =
        std::max(
            1,
            static_cast<int>(baseTime * timeMultiplier)
        );

    const double accuracy =
        std::min(0.95, baseAccuracy + accuracyBonus);

    if (money < cost)
    {
        return false;
    }

    money -= cost;

    explorationProject.start(
        duration,
        cost,
        accuracy
    );

    return true;
}

void Company::advanceExploration(double randomRoll)
{
    if (!explorationProject.isActive())
    {
        return;
    }

    const bool completed =
        explorationProject.advance(randomRoll);

    if (!completed)
    {
        return;
    }

    if (explorationProject.getResult() ==
        ExplorationResult::CommercialDiscovery)
    {
        // The exploration result creates knowledge of one reservoir.
        // These properties are intentionally hidden from the player
        // until discovery occurs.

        const double reserves = 1000000.0;
        const double pressure = 250.0;
        const double depth = 2800.0;

        reservoirs.push_back(
            std::make_unique<Reservoir>(
                nextReservoirId,
                "Field " + std::to_string(nextReservoirId),
                reserves,
                pressure,
                depth
            )
        );

        ++nextReservoirId;
        hasCommercialDiscovery = true;
    }
}

bool Company::startDrilling()
{
    if (!hasCommercialDiscovery)
    {
        return false;
    }

    if (drillingProject.isActive())
    {
        return false;
    }

    if (reservoirs.empty())
    {
        return false;
    }

    Reservoir* reservoir = reservoirs.back().get();

    const double maximumDepth =
        technologies.getEffect(
            TechnologyCategory::Drilling,
            TechnologyMetric::MaximumDepth
        );

    if (reservoir->getDepth() > maximumDepth)
    {
        return false;
    }

    const double costMultiplier =
        technologies.getEffect(
            TechnologyCategory::Drilling,
            TechnologyMetric::Cost
        );

    const double timeMultiplier =
        technologies.getEffect(
            TechnologyCategory::Drilling,
            TechnologyMetric::Time
        );

    const double capabilityMultiplier =
        technologies.getEffect(
            TechnologyCategory::Drilling,
            TechnologyMetric::WellCapability
        );

    const double baseCost = 70000.0;
    const int baseTime = 10;
    const double baseCapability = 300.0;

    // Depth contributes to drilling difficulty.
    const double depthFactor =
        1.0 + (reservoir->getDepth() / 10000.0);

    const double cost =
        baseCost *
        depthFactor *
        costMultiplier;

    const int duration =
        std::max(
            1,
            static_cast<int>(
                baseTime *
                depthFactor *
                timeMultiplier
            )
        );

    const double capability =
        baseCapability *
        capabilityMultiplier;

    if (money < cost)
    {
        return false;
    }

    money -= cost;

    drillingProject.start(
        reservoir,
        duration,
        cost,
        capability
    );

    return true;
}

void Company::advanceDrilling()
{
    if (!drillingProject.isActive())
    {
        return;
    }

    const bool completed =
        drillingProject.advance();

    if (!completed)
    {
        return;
    }

    Reservoir* reservoir =
        drillingProject.getReservoir();

    if (reservoir == nullptr)
    {
        return;
    }

    const double thresholdPressure = 180.0;
    const double cutoffPressure = 100.0;

    wells.push_back(
        std::make_unique<Well>(
            nextWellId,
            reservoir,
            drillingProject.getWellCapability(),
            thresholdPressure,
            cutoffPressure
        )
    );

    ++nextWellId;

    
    drillingProject.reset();
}

bool Company::buildTransportation()
{
    if (transportation.isBuilt() ||
        transportation.isUnderConstruction())
    {
        return false;
    }

    const double capacityMultiplier =
        technologies.getEffect(
            TechnologyCategory::Transportation,
            TechnologyMetric::Capacity
        );

    const double costMultiplier =
        technologies.getEffect(
            TechnologyCategory::Transportation,
            TechnologyMetric::CostPerBarrel
        );

    const double range =
        technologies.getEffect(
            TechnologyCategory::Transportation,
            TechnologyMetric::Range
        );

    const double capacity =
        1000.0 * capacityMultiplier;

    const double cost = 40000.0;
    const int constructionTime = 5;

    if (money < cost)
    {
        return false;
    }

    money -= cost;

    transportation.startConstruction(
        constructionTime,
        cost,
        capacity,
        2.0 * costMultiplier,
        range
    );

    return true;
}

bool Company::buildStorage()
{
    if (storage.isBuilt() ||
        storage.isUnderConstruction())
    {
        return false;
    }

    const double capacityMultiplier =
        technologies.getEffect(
            TechnologyCategory::Storage,
            TechnologyMetric::Capacity
        );

    const double costMultiplier =
        technologies.getEffect(
            TechnologyCategory::Storage,
            TechnologyMetric::CostPerBarrel
        );

    const double capacity =
        100000.0 * capacityMultiplier;

    const double cost = 50000.0;
    const int constructionTime = 5;

    if (money < cost)
    {
        return false;
    }

    money -= cost;

    storage.startConstruction(
        constructionTime,
        cost,
        capacity,
        0.50 * costMultiplier
    );

    return true;
}

void Company::advanceConstruction()
{
    transportation.advanceConstruction();
    storage.advanceConstruction();
}

void Company::processMaintenance()
{
    const double degradationMultiplier =
        technologies.getEffect(
            TechnologyCategory::Maintenance,
            TechnologyMetric::DegradationRate
        );

    // The first implementation uses a simple daily degradation
    // schedule. Technology can later make this probabilistic.
    static int degradationCounter = 0;

    ++degradationCounter;

    const int baseInterval = 10;

    const int effectiveInterval =
        std::max(
            1,
            static_cast<int>(
                baseInterval /
                degradationMultiplier
            )
        );

    if (degradationCounter < effectiveInterval)
    {
        return;
    }

    degradationCounter = 0;

    for (const auto& well : wells)
    {
        well->degrade();

        if (maintenancePolicy == MaintenancePolicy::AutoRepair)
        {
            repairWell(well->getId());
        }
        else if (
            maintenancePolicy ==
            MaintenancePolicy::AutoRepairBelow40
        )
        {
            if (
                well->getMaintenanceState() ==
                    MaintenanceState::Poor ||
                well->getMaintenanceState() ==
                    MaintenanceState::Critical ||
                well->getMaintenanceState() ==
                    MaintenanceState::Failed
            )
            {
                repairWell(well->getId());
            }
        }
    }
}

void Company::processRepairs()
{
    for (const auto& well : wells)
    {
        if (well->getStatus() !=
            WellStatus::UnderMaintenance)
        {
            continue;
        }

        well->processRepair();
    }
}

double Company::processProduction()
{
    double totalProduction = 0.0;

    for (const auto& well : wells)
    {
        const double production =
            well->calculateProduction();

        if (production <= 0.0)
        {
            continue;
        }

        well->getReservoir()->produce(production);

        totalProduction += production;
    }

    return totalProduction;
}

double Company::processTransportation(
    double producedOil
)
{
    if (!transportation.isBuilt())
    {
        return 0.0;
    }

    return transportation.transport(producedOil);
}

double Company::processStorage(
    double transportedOil
)
{
    if (!storage.isBuilt())
    {
        return 0.0;
    }

    return storage.addOil(transportedOil);
}

bool Company::repairWell(int wellId)
{
    for (const auto& well : wells)
    {
        if (well->getId() != wellId)
        {
            continue;
        }

        const MaintenanceState state =
            well->getMaintenanceState();

        int repairDays = 0;
        int sparePartsRequired = 0;

        switch (state)
        {
        case MaintenanceState::Good:
            repairDays = 1;
            sparePartsRequired = 2;
            break;

        case MaintenanceState::Fair:
            repairDays = 2;
            sparePartsRequired = 4;
            break;

        case MaintenanceState::Poor:
            repairDays = 3;
            sparePartsRequired = 6;
            break;

        case MaintenanceState::Critical:
            repairDays = 4;
            sparePartsRequired = 8;
            break;

        case MaintenanceState::Failed:
            repairDays = 5;
            sparePartsRequired = 10;
            break;

        case MaintenanceState::Excellent:
            return false;
        }

        const double partsMultiplier =
            technologies.getEffect(
                TechnologyCategory::Maintenance,
                TechnologyMetric::SparePartsConsumption
            );

        const double repairTimeMultiplier =
            technologies.getEffect(
                TechnologyCategory::Maintenance,
                TechnologyMetric::RepairTime
            );

        sparePartsRequired =
            std::max(
                1,
                static_cast<int>(
                    sparePartsRequired *
                    partsMultiplier
                )
            );

        repairDays =
            std::max(
                1,
                static_cast<int>(
                    repairDays *
                    repairTimeMultiplier
                )
            );

        if (spareParts < sparePartsRequired)
        {
            return false;
        }

        if (!well->startRepair(repairDays))
        {
            return false;
        }

        spareParts -= sparePartsRequired;

        return true;
    }

    return false;
}

bool Company::setMaintenancePolicy(
    MaintenancePolicy policy
)
{
    maintenancePolicy = policy;
    return true;
}

bool Company::sellOil(
    double barrels,
    double oilPrice
)
{
    if (barrels <= 0.0)
    {
        return false;
    }

    const double sold =
        storage.sellOil(barrels);

    if (sold <= 0.0)
    {
        return false;
    }

    const double revenue =
        sold * oilPrice;

    money += revenue;

    return true;
}

bool Company::buySpareParts(
    int amount,
    double pricePerPart
)
{
    if (amount <= 0)
    {
        return false;
    }

    const double totalCost =
        amount * pricePerPart;

    if (money < totalCost)
    {
        return false;
    }

    money -= totalCost;
    spareParts += amount;

    return true;
}

bool Company::unlockTechnology(
    const std::string& technologyId
)
{
    return technologies.unlockTechnology(
        technologyId,
        money
    );
}

bool Company::hasDiscovery() const
{
    return hasCommercialDiscovery;
}

void Company::printWells() const
{
    std::cout << "\n========== WELLS ==========\n";

    if (wells.empty())
    {
        std::cout << "No wells have been drilled.\n";
        return;
    }

    for (const auto& well : wells)
    {
        std::cout
            << "Well " << well->getId()
            << " | Field: "
            << well->getReservoir()->getName()
            << " | Production: "
            << std::fixed
            << std::setprecision(1)
            << well->calculateProduction()
            << " bbl/day"
            << " | Maintenance: "
            << maintenancePercentage(
                well->getMaintenanceState()
            )
            << "%"
            << " | Status: "
            << wellStatusToString(
                well->getStatus()
            )
            << '\n';
    }
}

void Company::printReservoirs() const
{
    std::cout << "\n======= RESERVOIRS ========\n";

    if (reservoirs.empty())
    {
        std::cout << "No commercial discoveries.\n";
        return;
    }

    for (const auto& reservoir : reservoirs)
    {
        std::cout
            << reservoir->getName()
            << " | Reserves: "
            << std::fixed
            << std::setprecision(0)
            << reservoir->getRemainingReserves()
            << " bbl"
            << " | Pressure: "
            << std::setprecision(1)
            << reservoir->getCurrentPressure()
            << " bar"
            << " | Depth: "
            << reservoir->getDepth()
            << " m\n";
    }
}

void Company::printTechnologies() const
{
    std::cout << "\n======= TECHNOLOGIES ======\n";

    for (const Technology& technology :
         technologies.getTechnologies())
    {
        std::cout
            << technology.id
            << " | "
            << technology.name
            << " | Tier "
            << technology.tier
            << " | Cost $"
            << std::fixed
            << std::setprecision(0)
            << technology.cost
            << " | ";

        if (technology.unlocked)
        {
            std::cout << "UNLOCKED";
        }
        else if (
            technology.prerequisiteId.empty() ||
            technologies.isUnlocked(
                technology.prerequisiteId
            )
        )
        {
            std::cout << "AVAILABLE";
        }
        else
        {
            std::cout << "LOCKED";
        }

        std::cout << '\n';
    }
}
