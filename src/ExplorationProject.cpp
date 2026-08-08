#include "ExplorationProject.h"

ExplorationProject::ExplorationProject()
    : status(ProjectStatus::NotStarted),
      result(ExplorationResult::None),
      daysRemaining(0),
      cost(0.0),
      accuracy(0.0)
{
}

bool ExplorationProject::isActive() const
{
    return status == ProjectStatus::InProgress;
}

bool ExplorationProject::isCompleted() const
{
    return status == ProjectStatus::Completed;
}

ProjectStatus ExplorationProject::getStatus() const
{
    return status;
}

ExplorationResult ExplorationProject::getResult() const
{
    return result;
}

int ExplorationProject::getDaysRemaining() const
{
    return daysRemaining;
}

double ExplorationProject::getCost() const
{
    return cost;
}

double ExplorationProject::getAccuracy() const
{
    return accuracy;
}

void ExplorationProject::start(
    int durationDays,
    double explorationCost,
    double explorationAccuracy
)
{
    status = ProjectStatus::InProgress;
    result = ExplorationResult::None;

    daysRemaining = durationDays;
    cost = explorationCost;
    accuracy = explorationAccuracy;
}

bool ExplorationProject::advance(double randomRoll)
{
    if (!isActive())
    {
        return false;
    }

    --daysRemaining;

    if (daysRemaining > 0)
    {
        return false;
    }

    daysRemaining = 0;
    status = ProjectStatus::Completed;

    // Accuracy is represented as the probability
    // of finding a commercial discovery.
    if (randomRoll <= accuracy)
    {
        result = ExplorationResult::CommercialDiscovery;
    }
    else
    {
        result = ExplorationResult::DryHole;
    }

    return true;
}

void ExplorationProject::reset()
{
    status = ProjectStatus::NotStarted;
    result = ExplorationResult::None;
    daysRemaining = 0;
    cost = 0.0;
    accuracy = 0.0;
}
