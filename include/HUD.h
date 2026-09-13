#pragma once

class Company;
struct SDL_Renderer;

class HUD
{
public:
    HUD();

    void render(
        SDL_Renderer* renderer,
        const Company& company
    );

private:
    void renderCompanyStatus(
        SDL_Renderer* renderer,
        const Company& company
    );

    void renderSimulationStatus(
        SDL_Renderer* renderer,
        const Company& company
    );

    void renderCompanySummary(
        SDL_Renderer* renderer,
        const Company& company
    );

    void renderText(
        SDL_Renderer* renderer,
        float x,
        float y,
        const char* text
    );

private:
    float screenWidth;
    float screenHeight;
};