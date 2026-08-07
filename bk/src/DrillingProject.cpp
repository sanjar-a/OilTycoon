#include "DrillingProject.h"

#include "Reservoir.h"

DrillingProject::DrillingProject()
    : status(ProjectStatus::NotStarted),
      reservoir(nullptr),
      daysRemaining(0),
      cost(0.0),
      wellCapability(0.0)
{
}

bool DrillingProject::isActive() const
{
    return status == ProjectStatus::InProgress;
}

bool DrillingProject::isCompleted() const
{
    return status == ProjectStatus::Completed;
}

ProjectStatus DrillingProject::getStatus() const
{
    return status;
}

Reservoir* DrillingProject::getReservoir()
{
    return reservoir;
}

int DrillingProject::getDaysRemaining() const
{
    return daysRemaining;
}

double DrillingProject::getCost() const
{
    return cost;
}

double DrillingProject::getWellCapability() const
{
    return wellCapability;
}

void DrillingProject::start(
    Reservoir* drillingReservoir,
    int durationDays,
    double drillingCost,
    double capability
)
{
    reservoir = drillingReservoir;
    daysRemaining = durationDays;
    cost = drillingCost;
    wellCapability = capability;

    status = ProjectStatus::InProgress;
}

bool DrillingProject::advance()
{
    if (!isActive())
    {
        return false;
    }

    --daysRemaining;

    if (daysRemaining <= 0)
    {
        daysRemaining = 0;
        status = ProjectStatus::Completed;

        return true;
    }

    return false;
}

void DrillingProject::reset()
{
    status = ProjectStatus::NotStarted;
    reservoir = nullptr;
    daysRemaining = 0;
    cost = 0.0;
    wellCapability = 0.0;
}
