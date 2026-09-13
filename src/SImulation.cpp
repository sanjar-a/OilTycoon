#include "Simulation.h"

Simulation::Simulation()
    : currentDay(1),
      timeOfDay(0.0),
      paused(true)
{
}

void Simulation::update(double deltaTime)
{
    if (paused)
    {
        return;
    }

    timeOfDay += deltaTime;

    while (timeOfDay >= 24.0)
    {
        timeOfDay -= 24.0;
        ++currentDay;
    }
}

void Simulation::pause()
{
    paused = true;
}

void Simulation::resume()
{
    paused = false;
}

bool Simulation::isPaused() const
{
    return paused;
}

int Simulation::getDay() const
{
    return currentDay;
}

double Simulation::getTimeOfDay() const
{
    return timeOfDay;
}