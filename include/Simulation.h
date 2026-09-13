#pragma once

class Simulation
{
public:
    Simulation();

    void update(double deltaTime);

    void pause();
    void resume();

    bool isPaused() const;

    int getDay() const;
    double getTimeOfDay() const;

private:
    int currentDay;
    double timeOfDay;
    bool paused;
};