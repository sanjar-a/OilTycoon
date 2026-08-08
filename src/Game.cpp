#include "Game.h"

#include <iomanip>
#include <iostream>
#include <limits>

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

    while (running)
    {
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
        << "8. Sell 15,000 bbl\n"
        << "9. Buy Spare Parts\n"
        << "10. Show Status\n"
        << "11. Show Wells\n"
        << "12. Show Reservoirs\n"
        << "13. Show Technologies\n"
        << "14. Unlock Technology\n"
        << "0. Exit\n";
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
        showWells();
        break;

    case 12:
        showReservoirs();
        break;

    case 13:
        showTechnologies();
        break;

    case 14:
        unlockTechnology();
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
    if (company.startDrilling())
    {
        std::cout
            << "Drilling started.\n"
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
            << "Cannot start drilling.\n"
            << "Check that you have a commercial discovery,\n"
            << "sufficient money, and enough drilling depth.\n";
    }
}

void Game::buildTransportation()
{
    if (company.buildTransportation())
    {
        std::cout
            << "Transportation network construction started.\n";
    }
    else
    {
        std::cout
            << "Cannot build transportation network.\n";
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
    constexpr double salesVolume = 15000.0;

    if (company.sellOil(
            salesVolume,
            oilPricePerBarrel
        ))
    {
        std::cout
            << "Sold "
            << salesVolume
            << " bbl for $"
            << salesVolume * oilPricePerBarrel
            << ".\n";
    }
    else
    {
        std::cout
            << "Could not sell 15,000 bbl.\n"
            << "Check storage inventory.\n";
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
        << "Transportation: ";

    if (company.getTransportation().isBuilt())
    {
        std::cout
            << "BUILT | Capacity: "
            << company.getTransportation()
                   .getCapacityPerDay()
            << " bbl/day\n";
    }
    else if (
        company.getTransportation()
            .isUnderConstruction()
    )
    {
        std::cout
            << "UNDER CONSTRUCTION | "
            << company.getTransportation()
                   .getConstructionDaysRemaining()
            << " days remaining\n";
    }
    else
    {
        std::cout << "NOT BUILT\n";
    }

    std::cout
        << "Storage: ";

    if (company.getStorage().isBuilt())
    {
        std::cout
            << "BUILT | Inventory: "
            << company.getStorage().getInventory()
            << " / "
            << company.getStorage().getCapacity()
            << " bbl\n";
    }
    else if (
        company.getStorage()
            .isUnderConstruction()
    )
    {
        std::cout
            << "UNDER CONSTRUCTION | "
            << company.getStorage()
                   .getConstructionDaysRemaining()
            << " days remaining\n";
    }
    else
    {
        std::cout << "NOT BUILT\n";
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
