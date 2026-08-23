#include "Game.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

Game::Game()
    : currentDay(1),
      oilPricePerBarrel(70.0),
      company(),
      randomGenerator(12345),
      randomDistribution(0.0, 1.0),
      running(true)
{
}

void Game::run()
{
    std::cout
        << "========================================\n"
        << "           OIL TYCOON PROTOTYPE\n"
        << "========================================\n"
        << "Build your oil company from exploration\n"
        << "to production and sales.\n\n";

    if (!renderer.initialize())
    {
        return;
    }

    while (running)
    {
        renderer.processEvents(running);
        
        displayMainMenu();

        int command = 0;

        std::cout << "\nSelect action: ";

        if (!(std::cin >> command))
        {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << "Invalid input.\n";
            continue;
        }

        processCommand(command);
    }

    renderer.shutdown();
}

void Game::displayMainMenu() const
{
    std::cout
        << "\n========================================\n"
        << "Day: "
        << currentDay
        << "\nMoney: $"
        << std::fixed
        << std::setprecision(2)
        << company.getMoney()
        << "\nSpare Parts: "
        << company.getSpareParts()
        << "\nOil Price: $"
        << oilPricePerBarrel
        << "/bbl\n"
        << "========================================\n\n"

        << "1. Advance One Day\n"
        << "2. Start Exploration\n"
        << "3. Start Drilling\n"
        << "4. Build Transportation Network\n"
        << "5. Build Storage Facility\n"
        << "6. Set Maintenance Policy\n"
        << "7. Repair Well\n"
        << "8. Sell oil\n"
        << "9. Buy Spare Parts\n"
        << "10. Show Status\n"
        << "11. Financial Status\n"
        << "12. Show Wells\n"
        << "13. Show Reservoirs\n"
        << "14. Show Technologies\n"
        << "15. Unlock Technology\n"
        << "16. Manage Infrastructure\n"
        << "0. Exit\n";
}

void Game::manageInfrastructure()
{
    while (true)
    {
        std::cout
            << "\n========== INFRASTRUCTURE ==========\n"
            << "1. Show storage facilities\n"
            << "2. Upgrade storage capacity\n"
            << "3. Upgrade storage cost\n"
            << "4. Show transportation networks\n"
            << "5. Upgrade transportation capacity\n"
            << "6. Upgrade transportation cost\n"
            << "7. Upgrade transportation range\n"
            << "0. Back\n"
            << "Select: ";

        int choice = 0;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            showStorageFacilities();
            break;

        case 2:
            upgradeStorageCapacity();
            break;

        case 3:
            upgradeStorageCost();
            break;

        case 4:
            showTransportationNetworks();
            break;

        case 5:
            upgradeTransportationCapacity();
            break;

        case 6:
            upgradeTransportationCost();
            break;

        case 7:
            upgradeTransportationRange();
            break;

        case 0:
            return;

        default:
            std::cout
                << "Invalid option.\n";
        }
    }
}

void Game::processCommand(int command)
{
    switch (command)
    {
    case 1:
        advanceDay();
        break;

    case 2:
        startExploration();
        break;

    case 3:
        startDrilling();
        break;

    case 4:
        buildTransportation();
        break;

    case 5:
        buildStorage();
        break;

    case 6:
        setMaintenancePolicy();
        break;

    case 7:
        repairWell();
        break;

    case 8:
        sellOil();
        break;

    case 9:
        buySpareParts();
        break;

    case 10:
        showStatus();
        break;

    case 11:
        showFinancialStatus();
        break;

    case 12:
        showWells();
        break;

    case 13:
        showReservoirs();
        break;

    case 14:
        showTechnologies();
        break;

    case 15:
        unlockTechnology();
        break;

    case 16:
        manageInfrastructure();
        break;

    case 0:
        running = false;
        std::cout << "Game ended.\n";
        break;

    default:
        std::cout << "Unknown command.\n";
        break;
    }
}

void Game::advanceDay()
{
    company.resetDailyFinancials();
    std::cout
        << "\n========== ADVANCING DAY "
        << currentDay
        << " ==========\n";

    // 1. Exploration
    if (company.getExplorationProject().isActive())
    {
        const double roll = randomDistribution(randomGenerator);

        company.advanceExploration(roll);

        if (company.getExplorationProject().isCompleted())
        {
            if (
                company.getExplorationProject().getResult() ==
                ExplorationResult::CommercialDiscovery
            )
            {
                std::cout
                    << "Exploration completed: "
                    << "COMMERCIAL DISCOVERY!\n";
            }
            else
            {
                std::cout
                    << "Exploration completed: DRY HOLE.\n";
            }
        }
    }

    // 2. Drilling
    if (company.getDrillingProject().isActive())
    {
        company.advanceDrilling();

        if (!company.getDrillingProject().isActive() &&
            !company.getWells().empty())
        {
            std::cout
                << "Drilling completed. "
                << "A new well is now available.\n";
        }
    }

    // 3-4. Construction
    company.advanceConstruction();

    // 5. Maintenance degradation
    company.processMaintenance();

    // 6. Repairs
    company.processRepairs();

    // 7. Production
    const double produced =
        company.processProduction();

    // 8. Transportation
    const double transported =
        company.processTransportation(produced);

    // 9. Storage
    const double stored =
        company.processStorage(transported);

    company.processStorageCosts();

    std::cout
        << std::fixed
        << std::setprecision(1)
        << "Produced:    "
        << produced
        << " bbl\n"
        << "Transported: "
        << transported
        << " bbl\n"
        << "Stored:      "
        << stored
        << " bbl\n";

    company.printTransportationStatus();

    if (produced > transported)
    {
        std::cout
            << "Warning: "
            << produced - transported
            << " bbl could not be transported.\n";
    }

    if (transported > stored)
    {
        std::cout
            << "Warning: "
            << transported - stored
            << " bbl could not fit into storage.\n";
    }

    ++currentDay;
    updateOilPrice();
}

void Game::startExploration()
{
    if (company.startExploration())
    {
        std::cout
            << "Exploration started.\n"
            << "Duration: "
            << company.getExplorationProject()
                   .getDaysRemaining()
            << " days\n"
            << "Cost: $"
            << company.getExplorationProject()
                   .getCost()
            << "\n"
            << "Accuracy: "
            << company.getExplorationProject()
                   .getAccuracy() * 100.0
            << "%\n";
    }
    else
    {
        std::cout
            << "Cannot start exploration.\n";
    }
}

void Game::startDrilling()
{
    const auto& reservoirs =
        company.getReservoirs();

    if (reservoirs.empty())
    {
        std::cout
            << "No commercial discoveries available.\n";
        return;
    }

    std::cout
        << "\n========== SELECT FIELD TO DRILL ==========\n";

    for (const auto& reservoir : reservoirs)
    {
        const Location& location =
            reservoir->getLocation();

        const double distanceFromHQ =
            company.getHeadquartersLocation()
                .distanceTo(location);

        std::cout
            << "\nField "
            << reservoir->getId()
            << "\n"
            << "  Location: X="
            << std::fixed
            << std::setprecision(1)
            << location.getX()
            << " km, Y="
            << location.getY()
            << " km\n"
            << "  Distance from HQ: "
            << distanceFromHQ
            << " km\n"
            << "  Reserves: "
            << reservoir->getRemainingReserves()
            << " bbl\n"
            << "  Pressure: "
            << reservoir->getCurrentPressure()
            << " bar\n"
            << "  Depth: "
            << reservoir->getDepth()
            << " m\n";
    }

    std::cout
        << "\nEnter Field ID to drill: ";

    int reservoirId = 0;
    std::cin >> reservoirId;

    if (company.startDrilling(reservoirId))
    {
        std::cout
            << "Drilling started for Field "
            << reservoirId
            << ".\n"
            << "Duration: "
            << company.getDrillingProject()
                   .getDaysRemaining()
            << " days\n"
            << "Cost: $"
            << company.getDrillingProject()
                   .getCost()
            << "\n"
            << "Well capability: "
            << company.getDrillingProject()
                   .getWellCapability()
            << " bbl/day\n";
    }
    else
    {
        std::cout
            << "Cannot start drilling for Field "
            << reservoirId
            << ".\n"
            << "Check that the field exists, you have sufficient money,\n"
            << "and your drilling technology supports its depth.\n";
    }
}

void Game::buildTransportation()
{
    const auto& reservoirs =
        company.getReservoirs();

    if (reservoirs.empty())
    {
        std::cout
            << "No fields available for transportation.\n";
        return;
    }

    std::cout
        << "\n========== BUILD TRANSPORTATION ==========\n";

    for (const auto& reservoir : reservoirs)
    {
        const Location& location =
            reservoir->getLocation();

        const double distanceFromHQ =
            company.getHeadquartersLocation()
                .distanceTo(location);

        const double estimatedCost =
            40000.0 +
            distanceFromHQ * 500.0;

        std::cout
            << "\nField "
            << reservoir->getId()
            << "\n"
            << "  Location: X="
            << std::fixed
            << std::setprecision(1)
            << location.getX()
            << " km, Y="
            << location.getY()
            << " km\n"
            << "  Distance from HQ: "
            << distanceFromHQ
            << " km\n"
            << "  Estimated construction cost: $"
            << estimatedCost
            << "\n";
    }

    std::cout
        << "\nEnter Field ID: ";

    int reservoirId = 0;
    std::cin >> reservoirId;

    if (company.buildTransportation(reservoirId))
    {
        std::cout
            << "Transportation network construction started.\n";
    }
    else
    {
        std::cout
            << "Cannot build transportation network for Field "
            << reservoirId
            << ".\n"
            << "Check that the field exists and you have enough money.\n";
    }
}

void Game::buildStorage()
{
    if (company.buildStorage())
    {
        std::cout
            << "Storage facility construction started.\n";
    }
    else
    {
        std::cout
            << "Cannot build storage facility.\n";
    }
}

void Game::setMaintenancePolicy()
{
    std::cout
        << "\nMaintenance Policies:\n"
        << "1. Manual\n"
        << "2. AutoRepair\n"
        << "3. AutoRepairBelow40\n"
        << "Select: ";

    int choice = 0;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        company.setMaintenancePolicy(
            MaintenancePolicy::Manual
        );
        break;

    case 2:
        company.setMaintenancePolicy(
            MaintenancePolicy::AutoRepair
        );
        break;

    case 3:
        company.setMaintenancePolicy(
            MaintenancePolicy::AutoRepairBelow40
        );
        break;

    default:
        std::cout << "Invalid policy.\n";
        return;
    }

    std::cout
        << "Maintenance policy changed to "
        << maintenancePolicyToString(
            company.getMaintenancePolicy()
        )
        << ".\n";
}

void Game::repairWell()
{
    if (company.getWells().empty())
    {
        std::cout << "No wells available.\n";
        return;
    }

    showWells();

    std::cout << "Enter Well ID: ";

    int wellId = 0;
    std::cin >> wellId;

    if (company.repairWell(wellId))
    {
        std::cout
            << "Repair started successfully.\n";
    }
    else
    {
        std::cout
            << "Repair could not be started.\n"
            << "Check maintenance state and spare parts.\n";
    }
}

void Game::sellOil()
{
    std::cout
        << "\n========== SELL OIL ==========\n"
        << "1. Sell all available oil\n"
        << "2. Sell specific volume\n"
        << "0. Back\n"
        << "Select: ";

    int choice = 0;
    std::cin >> choice;

    if (choice == 0)
    {
        return;
    }

    double requestedVolume = 0.0;

    if (choice == 1)
    {
        // Calculate total oil across all built storage facilities.
        for (const auto& storage :
             company.getStorageFacilities())
        {
            if (storage->isBuilt())
            {
                requestedVolume +=
                    storage->getInventory();
            }
        }
    }
    else if (choice == 2)
    {
        std::cout
            << "Enter volume to sell (bbl): ";

        std::cin >> requestedVolume;
    }
    else
    {
        std::cout
            << "Invalid option.\n";
        return;
    }

    if (requestedVolume <= 0.0)
    {
        std::cout
            << "Volume must be greater than zero.\n";
        return;
    }

    // Calculate total oil actually available
    // across all built storage facilities.
    double availableOil = 0.0;

    for (const auto& storage :
         company.getStorageFacilities())
    {
        if (storage->isBuilt())
        {
            availableOil +=
                storage->getInventory();
        }
    }

    if (availableOil <= 0.0)
    {
        std::cout
            << "No oil is available for sale.\n";
        return;
    }

    // If the requested volume exceeds available oil,
    // Company::sellOil() will sell everything available.
    const double expectedSale =
        std::min(
            requestedVolume,
            availableOil
        );

    if (expectedSale <= 0.0)
    {
        std::cout
            << "No oil is available for sale.\n";
        return;
    }

    if (company.sellOil(
            requestedVolume,
            oilPricePerBarrel
        ))
    {
        std::cout
            << std::fixed
            << std::setprecision(1)
            << "Sold "
            << expectedSale
            << " bbl for $"
            << expectedSale * oilPricePerBarrel
            << ".\n";
    }
    else
    {
        std::cout
            << "Could not sell oil.\n";
    }
}

void Game::buySpareParts()
{
    std::cout
        << "Enter number of spare parts: ";

    int amount = 0;
    std::cin >> amount;

    constexpr double pricePerPart = 100.0;

    if (company.buySpareParts(
            amount,
            pricePerPart
        ))
    {
        std::cout
            << "Purchased "
            << amount
            << " spare parts for $"
            << amount * pricePerPart
            << ".\n";
    }
    else
    {
        std::cout
            << "Purchase failed.\n";
    }
}

void Game::showStatus() const
{
    std::cout
        << "\n========== COMPANY STATUS ==========\n"
        << "Day: "
        << currentDay
        << "\nMoney: $"
        << company.getMoney()
        << "\nSpare Parts: "
        << company.getSpareParts()
        << "\nMaintenance Policy: "
        << maintenancePolicyToString(
               company.getMaintenancePolicy()
           )
        << "\n\n";

    std::cout
        << "Oil Price: $"
        << std::fixed
        << std::setprecision(2)
        << oilPricePerBarrel
        << " / bbl\n";

    // =========================
    // TRANSPORTATION
    // =========================

    const auto& transportationNetworks =
        company.getTransportationNetworks();

    int builtTransportation = 0;
    int constructionTransportation = 0;

    double totalTransportCapacity = 0.0;

    int constructionDaysRemaining = 0;

    for (const auto& network :
         transportationNetworks)
    {
        if (network->isBuilt())
        {
            ++builtTransportation;

            totalTransportCapacity +=
                network->getCapacityPerDay();
        }
        else if (network->isUnderConstruction())
        {
            ++constructionTransportation;

            constructionDaysRemaining =
                std::max(
                    constructionDaysRemaining,
                    network->getConstructionDaysRemaining()
                );
        }
    }

    std::cout
        << "Transportation: ";

    if (builtTransportation > 0)
    {
        std::cout
            << builtTransportation
            << " BUILT | Total Capacity: "
            << totalTransportCapacity
            << " bbl/day\n";
    }
    else if (constructionTransportation > 0)
    {
        std::cout
            << constructionTransportation
            << " UNDER CONSTRUCTION | "
            << constructionDaysRemaining
            << " days remaining\n";
    }
    else
    {
        std::cout
            << "NOT BUILT\n";
    }

    // =========================
    // STORAGE
    // =========================

    const auto& storageFacilities =
        company.getStorageFacilities();

    int builtStorage = 0;
    int constructionStorage = 0;

    double totalStorageCapacity = 0.0;
    double totalStoredOil = 0.0;

    int storageConstructionDaysRemaining = 0;

    for (const auto& storage :
         storageFacilities)
    {
        if (storage->isBuilt())
        {
            ++builtStorage;

            totalStorageCapacity +=
                storage->getCapacity();

            totalStoredOil +=
                storage->getInventory();
        }
        else if (storage->isUnderConstruction())
        {
            ++constructionStorage;

            storageConstructionDaysRemaining =
                std::max(
                    storageConstructionDaysRemaining,
                    storage->getConstructionDaysRemaining()
                );
        }
    }

    std::cout
        << "Storage: ";

    if (builtStorage > 0)
    {
        std::cout
            << builtStorage
            << " BUILT | Inventory: "
            << totalStoredOil
            << " / "
            << totalStorageCapacity
            << " bbl\n";
    }
    else if (constructionStorage > 0)
    {
        std::cout
            << constructionStorage
            << " UNDER CONSTRUCTION | "
            << storageConstructionDaysRemaining
            << " days remaining\n";
    }
    else
    {
        std::cout
            << "NOT BUILT\n";
    }

    printProjectStatus();
}

void Game::showWells() const
{
    company.printWells();
}

void Game::showReservoirs() const
{
    company.printReservoirs();
}

void Game::showTechnologies() const
{
    company.printTechnologies();
}

void Game::unlockTechnology()
{
    company.printTechnologies();

    std::cout
        << "\nEnter technology ID: ";

    std::string technologyId;
    std::cin >> technologyId;

    if (company.unlockTechnology(technologyId))
    {
        std::cout
            << "Technology unlocked successfully.\n";
    }
    else
    {
        std::cout
            << "Technology could not be unlocked.\n"
            << "Check cost and prerequisites.\n";
    }
}

void Game::printProjectStatus() const
{
    std::cout
        << "\n========== PROJECTS ==========\n";

    if (company.getExplorationProject().isActive())
    {
        std::cout
            << "Exploration: "
            << company.getExplorationProject()
                   .getDaysRemaining()
            << " days remaining\n";
    }
    else
    {
        std::cout
            << "Exploration: inactive\n";
    }

    if (company.getDrillingProject().isActive())
    {
        std::cout
            << "Drilling: "
            << company.getDrillingProject()
                   .getDaysRemaining()
            << " days remaining\n";
    }
    else
    {
        std::cout
            << "Drilling: inactive\n";
    }
}

void Game::showStorageFacilities() const
{
    const auto& storages =
        company.getStorageFacilities();

    std::cout
        << "\n========== STORAGE FACILITIES ==========\n";

    if (storages.empty())
    {
        std::cout
            << "No storage facilities.\n";
        return;
    }

    for (const auto& storage : storages)
    {
        std::cout
            << "Storage #"
            << storage->getId()
            << " | ";

        if (storage->isBuilt())
        {
            std::cout
                << "BUILT"
                << " | Inventory: "
                << storage->getInventory()
                << " / "
                << storage->getCapacity()
                << " bbl"
                << " | Location: X="
                << storage->getLocation().getX()
                << " km, Y="
                << storage->getLocation().getY()
                << " km"
                << " | Capacity Tier: "
                << storage->getCapacityUpgradeTier()
                << " | Cost Tier: "
                << storage->getCostUpgradeTier()
                << '\n';
        }
        else if (storage->isUnderConstruction())
        {
            std::cout
                << "UNDER CONSTRUCTION | "
                << storage->getConstructionDaysRemaining()
                << " days remaining\n";
        }
        else
        {
            std::cout
                << "NOT BUILT\n";
        }
    }
}

void Game::upgradeStorageCapacity()
{
    showStorageFacilities();

    std::cout
        << "Enter Storage ID to upgrade: ";

    int id = 0;
    std::cin >> id;

    if (company.upgradeStorageCapacity(id))
    {
        std::cout
            << "Storage capacity upgraded successfully.\n";
    }
    else
    {
        std::cout
            << "Storage capacity upgrade failed.\n"
            << "Check that the facility exists, is built,\n"
            << "and the required technology is unlocked.\n";
    }
}

void Game::showTransportationNetworks() const
{
    const auto& networks =
        company.getTransportationNetworks();

    std::cout
        << "\n======= TRANSPORTATION NETWORKS =======\n";

    if (networks.empty())
    {
        std::cout
            << "No transportation networks.\n";
        return;
    }

    for (const auto& network : networks)
    {
        std::cout
            << "Transport #"
            << network->getId()
            << " | ";

        if (network->isBuilt())
        {
            std::cout
                << "BUILT"
                << " | Capacity: "
                << network->getCapacityPerDay()
                << " bbl/day"
                << " | Range: "
                << network->getRange()
                << " km"
                << " | Capacity Tier: "
                << network->getCapacityUpgradeTier()
                << " | Cost Tier: "
                << network->getCostUpgradeTier()
                << " | Range Tier: "
                << network->getRangeUpgradeTier()
                << '\n';
        }
        else if (network->isUnderConstruction())
        {
            std::cout
                << "UNDER CONSTRUCTION | "
                << network->getConstructionDaysRemaining()
                << " days remaining\n";
        }
        else
        {
            std::cout
                << "NOT BUILT\n";
        }
    }
}

void Game::upgradeStorageCost()
{
    std::cout
        << "\n========== UPGRADE STORAGE COST ==========\n";

    const auto& storages =
        company.getStorageFacilities();

    if (storages.empty())
    {
        std::cout
            << "No storage facilities available.\n";
        return;
    }

    bool hasBuiltStorage = false;

    for (const auto& storage : storages)
    {
        if (storage->isBuilt())
        {
            hasBuiltStorage = true;
            break;
        }
    }

    if (!hasBuiltStorage)
    {
        std::cout
            << "No built storage facilities available.\n";
        return;
    }

    std::cout
        << "Select storage facility to upgrade:\n";

    for (std::size_t i = 0; i < storages.size(); ++i)
    {
        if (storages[i]->isBuilt())
        {
            std::cout
                << i + 1
                << ". Storage #"
                << i + 1
                << " | Cost per barrel: "
                << storages[i]->getCostPerBarrel()
                << "\n";
        }
    }

    std::cout
        << "0. Back\n"
        << "Select: ";

    int choice = 0;
    std::cin >> choice;

    if (choice == 0)
    {
        return;
    }

    if (choice < 1 ||
        choice > static_cast<int>(storages.size()) ||
        !storages[choice - 1]->isBuilt())
    {
        std::cout
            << "Invalid storage facility.\n";
        return;
    }

    auto& storage =
        *storages[choice - 1];

    const double currentCost =
        storage.getCostPerBarrel();

    const double newCost =
        currentCost * 0.85;

    storage.setCostPerBarrel(newCost);

    std::cout
        << "Storage #"
        << choice
        << " cost per barrel upgraded from "
        << currentCost
        << " to "
        << newCost
        << ".\n";
}


void Game::upgradeTransportationCapacity()
{
    std::cout
        << "\n========== UPGRADE TRANSPORTATION CAPACITY ==========\n";

    const auto& networks =
        company.getTransportationNetworks();

    if (networks.empty())
    {
        std::cout
            << "No transportation networks available.\n";
        return;
    }

    bool hasBuiltNetwork = false;

    for (const auto& network : networks)
    {
        if (network->isBuilt())
        {
            hasBuiltNetwork = true;
            break;
        }
    }

    if (!hasBuiltNetwork)
    {
        std::cout
            << "No built transportation networks available.\n";
        return;
    }

    std::cout
        << "Select transportation network to upgrade:\n";

    for (std::size_t i = 0; i < networks.size(); ++i)
    {
        if (networks[i]->isBuilt())
        {
            std::cout
                << i + 1
                << ". Transportation #"
                << i + 1
                << " | Capacity: "
                << networks[i]->getCapacityPerDay()
                << " bbl/day\n";
        }
    }

    std::cout
        << "0. Back\n"
        << "Select: ";

    int choice = 0;
    std::cin >> choice;

    if (choice == 0)
    {
        return;
    }

    if (choice < 1 ||
        choice > static_cast<int>(networks.size()) ||
        !networks[choice - 1]->isBuilt())
    {
        std::cout
            << "Invalid transportation network.\n";
        return;
    }

    auto& network =
        *networks[choice - 1];

    const double currentCapacity =
    network.getCapacityPerDay();

    const int newTier =
        network.getCapacityUpgradeTier() + 1;

    if (network.upgradeCapacity(
            1.25,
            newTier
        ))
    {
        std::cout
            << "Transportation #"
            << choice
            << " capacity upgraded from "
            << currentCapacity
            << " to "
            << network.getCapacityPerDay()
            << " bbl/day.\n"
            << "Capacity Tier: "
            << network.getCapacityUpgradeTier()
            << "\n";
    }
    else
    {
        std::cout
            << "Transportation capacity upgrade failed.\n";
    }
}


void Game::upgradeTransportationCost()
{
    std::cout
        << "\n========== UPGRADE TRANSPORTATION COST ==========\n";

    const auto& networks =
        company.getTransportationNetworks();

    if (networks.empty())
    {
        std::cout
            << "No transportation networks available.\n";
        return;
    }

    bool hasBuiltNetwork = false;

    for (const auto& network : networks)
    {
        if (network->isBuilt())
        {
            hasBuiltNetwork = true;
            break;
        }
    }

    if (!hasBuiltNetwork)
    {
        std::cout
            << "No built transportation networks available.\n";
        return;
    }

    std::cout
        << "Select transportation network to upgrade:\n";

    for (std::size_t i = 0; i < networks.size(); ++i)
    {
        if (networks[i]->isBuilt())
        {
            std::cout
                << i + 1
                << ". Transportation #"
                << i + 1
                << " | Cost per barrel: "
                << networks[i]->getCostPerBarrel()
                << "\n";
        }
    }

    std::cout
        << "0. Back\n"
        << "Select: ";

    int choice = 0;
    std::cin >> choice;

    if (choice == 0)
    {
        return;
    }

    if (choice < 1 ||
        choice > static_cast<int>(networks.size()) ||
        !networks[choice - 1]->isBuilt())
    {
        std::cout
            << "Invalid transportation network.\n";
        return;
    }

    auto& network =
        *networks[choice - 1];

    const double currentCost =
        network.getCostPerBarrel();

    const double newCost =
        currentCost * 0.85;


    std::cout
        << "Transportation #"
        << choice
        << " cost per barrel upgraded from "
        << currentCost
        << " to "
        << newCost
        << ".\n";
}


void Game::upgradeTransportationRange()
{
    std::cout
        << "\n========== UPGRADE TRANSPORTATION RANGE ==========\n";

    const auto& networks =
        company.getTransportationNetworks();

    if (networks.empty())
    {
        std::cout
            << "No transportation networks available.\n";
        return;
    }

    bool hasBuiltNetwork = false;

    for (const auto& network : networks)
    {
        if (network->isBuilt())
        {
            hasBuiltNetwork = true;
            break;
        }
    }

    if (!hasBuiltNetwork)
    {
        std::cout
            << "No built transportation networks available.\n";
        return;
    }

    std::cout
        << "Select transportation network to upgrade:\n";

    for (std::size_t i = 0; i < networks.size(); ++i)
    {
        if (networks[i]->isBuilt())
        {
            std::cout
                << i + 1
                << ". Transportation #"
                << i + 1
                << " | Range: "
                << networks[i]->getRange()
                << " km\n";
        }
    }

    std::cout
        << "0. Back\n"
        << "Select: ";

    int choice = 0;
    std::cin >> choice;

    if (choice == 0)
    {
        return;
    }

    if (choice < 1 ||
        choice > static_cast<int>(networks.size()) ||
        !networks[choice - 1]->isBuilt())
    {
        std::cout
            << "Invalid transportation network.\n";
        return;
    }

    auto& network =
        *networks[choice - 1];

    const double currentRange =
        network.getRange();

    const double newRange =
        currentRange + 150.0;

    network.setRange(newRange);

    std::cout
        << "Transportation #"
        << choice
        << " range upgraded from "
        << currentRange
        << " to "
        << newRange
        << " km.\n";
}

void Game::updateOilPrice()
{
    const int change = std::rand() % 3 - 1; // -1, 0, +1

    oilPricePerBarrel += static_cast<double>(change);

    if (oilPricePerBarrel < MIN_OIL_PRICE)
    {
        oilPricePerBarrel = MIN_OIL_PRICE;
    }
    else if (oilPricePerBarrel > MAX_OIL_PRICE)
    {
        oilPricePerBarrel = MAX_OIL_PRICE;
    }
}

void Game::showFinancialStatus() const
{
    const double revenue =
        company.getDailyRevenue();

    const double drilling =
        company.getDailyDrillingExpenses();

    const double exploration =
        company.getDailyExplorationExpenses();

    const double construction =
        company.getDailyConstructionExpenses();

    const double technology =
        company.getDailyTechnologyExpenses();

    const double transportation =
        company.getDailyTransportationExpenses();

    const double storage =
        company.getDailyStorageExpenses();

    const double spareParts =
        company.getDailySparePartsExpenses();

    const double totalExpenses =
        company.getDailyTotalExpenses();

    const double profit =
        company.getDailyProfit();

    std::cout
        << "\n========== FINANCIAL STATUS ==========\n"
        << std::fixed
        << std::setprecision(2)

        << "Revenue\n"
        << "  Oil Sales:          $" << revenue << "\n\n"

        << "Expenses\n"
        << "  Exploration:        $" << exploration << "\n"
        << "  Drilling:           $" << drilling << "\n"
        << "  Construction:       $" << construction << "\n"
        << "  Technology:         $" << technology << "\n"
        << "  Transportation:     $" << transportation << "\n"
        << "  Storage:             $" << storage << "\n"
        << "  Spare Parts:        $" << spareParts << "\n"
        << "  -------------------------------\n"
        << "  Total Expenses:     $" << totalExpenses << "\n\n"

        << "Net Profit/Loss:       $" << profit << "\n"
        << "Current Cash:          $" << company.getMoney()
        << "\n";
}