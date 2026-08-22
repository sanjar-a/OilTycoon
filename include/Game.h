#ifndef OILTYCOON_GAME_H
#define OILTYCOON_GAME_H

#include "Company.h"

#include <random>
#include <SDL3/SDL.h>
class Game
{
private:
    int currentDay;

    double oilPricePerBarrel;
    static constexpr double MIN_OIL_PRICE = 65.0;
    static constexpr double MAX_OIL_PRICE = 75.0;

    Company company;

    std::mt19937 randomGenerator;
    std::uniform_real_distribution<double> randomDistribution;

    bool running;

public:
    Game();

    void run();

private:
    void displayMainMenu() const;
    void processCommand(int command);

    void advanceDay();

    void startExploration();
    void startDrilling();

    void buildTransportation();
    void buildStorage();

    void setMaintenancePolicy();
    void repairWell();

    void sellOil();
    void buySpareParts();

    void showStatus() const;
    void showWells() const;
    void showReservoirs() const;
    void showTechnologies() const;
    void unlockTechnology();

    void printProjectStatus() const;

    void manageInfrastructure();

    void upgradeStorageCapacity();
    void upgradeStorageCost();

    void upgradeTransportationCapacity();
    void upgradeTransportationCost();
    void upgradeTransportationRange();
    void showStorageFacilities() const;
    void showTransportationNetworks() const;
    void updateOilPrice();

    void showFinancialStatus() const;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool sdlInitialized = false;

    bool initializeSDL();
    void shutdownSDL();
    void processSDLEvents();
};

#endif
