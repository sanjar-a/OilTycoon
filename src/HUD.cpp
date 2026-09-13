#include "HUD.h"
#include "Company.h"

#include <SDL3/SDL.h>
#include <cstdio>

HUD::HUD()
    : screenWidth(1280.0f),
      screenHeight(720.0f)
{
}

void HUD::render(
    SDL_Renderer* renderer,
    const Company& company
)
{
    renderCompanyStatus(
        renderer,
        company
    );

    renderSimulationStatus(
        renderer,
        company
    );

    renderCompanySummary(
        renderer,
        company
    );
}

void HUD::renderCompanyStatus(
    SDL_Renderer* renderer,
    const Company& company
)
{
    SDL_FRect panel{
        0.0f,
        0.0f,
        screenWidth,
        55.0f
    };

    SDL_SetRenderDrawColor(
        renderer,
        15,
        18,
        24,
        235
    );

    SDL_RenderFillRect(
        renderer,
        &panel
    );

    char buffer[128];

    std::snprintf(
        buffer,
        sizeof(buffer),
        "OIL COMPANY    Money: $%.0f",
        company.getMoney()
    );

    renderText(
        renderer,
        20.0f,
        18.0f,
        buffer
    );
}

void HUD::renderSimulationStatus(
    SDL_Renderer* renderer,
    const Company& company
)
{
    (void)company;

    renderText(
        renderer,
        screenWidth - 220.0f,
        18.0f,
        "DAY 1    PAUSED"
    );
}

void HUD::renderCompanySummary(
    SDL_Renderer* renderer,
    const Company& company
)
{
    const float panelHeight = 70.0f;

    SDL_FRect panel{
        0.0f,
        screenHeight - panelHeight,
        screenWidth,
        panelHeight
    };

    SDL_SetRenderDrawColor(
        renderer,
        15,
        18,
        24,
        235
    );

    SDL_RenderFillRect(
        renderer,
        &panel
    );

    char buffer[128];

    std::snprintf(
        buffer,
        sizeof(buffer),
        "WELLS: %zu",
        company.getWells().size()
    );

    renderText(
        renderer,
        20.0f,
        screenHeight - 45.0f,
        buffer
    );

    std::snprintf(
        buffer,
        sizeof(buffer),
        "RESERVOIRS: %zu",
        company.getReservoirs().size()
    );

    renderText(
        renderer,
        150.0f,
        screenHeight - 45.0f,
        buffer
    );

    std::snprintf(
        buffer,
        sizeof(buffer),
        "STORAGE: %zu",
        company.getStorageFacilities().size()
    );

    renderText(
        renderer,
        330.0f,
        screenHeight - 45.0f,
        buffer
    );

    std::snprintf(
        buffer,
        sizeof(buffer),
        "TRANSPORT: %zu",
        company.getTransportationNetworks().size()
    );

    renderText(
        renderer,
        500.0f,
        screenHeight - 45.0f,
        buffer
    );
}

void HUD::renderText(
    SDL_Renderer* renderer,
    float x,
    float y,
    const char* text
)
{
    SDL_RenderDebugText(
        renderer,
        x,
        y,
        text
    );
}