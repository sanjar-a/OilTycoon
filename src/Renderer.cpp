#include "Renderer.h"
#include "Company.h"
#include <cstdio>
#include <iostream>

Renderer::Renderer()
    : window(nullptr),
      renderer(nullptr),
      cameraX(0.0f),
      cameraY(0.0f),
      zoom(5.0f),
      isDragging(false),
      mouseMovedDuringClick(false),
      lastMouseX(0.0f),
      lastMouseY(0.0f),
      mouseDownX(0.0f),
      mouseDownY(0.0f),
      screenWidth(1280),
      screenHeight(720),
      selectedObjectType(SelectedObjectType::None),
      selectedObjectId(-1)
{
}

Renderer::~Renderer()
{
    shutdown();
}

bool Renderer::initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr
            << "SDL initialization failed: "
            << SDL_GetError()
            << '\n';

        return false;
    }

    window = SDL_CreateWindow(
        "OilTycoon",
        1280,
        720,
        SDL_WINDOW_RESIZABLE
    );

    if (window == nullptr)
    {
        std::cerr
            << "SDL window creation failed: "
            << SDL_GetError()
            << '\n';

        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        nullptr
    );

    if (renderer == nullptr)
    {
        std::cerr
            << "SDL renderer creation failed: "
            << SDL_GetError()
            << '\n';

        SDL_DestroyWindow(window);
        window = nullptr;

        SDL_Quit();
        return false;
    }

    return true;
}

void Renderer::shutdown()
{
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

void Renderer::processEvents(
    bool& running,
    const Company& company
)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_EVENT_MOUSE_WHEEL)
        {
            handleMouseWheel(
                event.wheel.y
            );
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                handleMouseButtonDown(
                    event.button.x,
                    event.button.y
                );
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                handleMouseButtonUp(
                    event.button.x,
                    event.button.y,
                    company
                );
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            handleMouseMotion(
                event.motion.x,
                event.motion.y
            );
        }
    }

    const bool* keyboardState =
        SDL_GetKeyboardState(nullptr);

    handleKeyboard(keyboardState);
}

void Renderer::render(const Company& company)
{
    SDL_GetRenderOutputSize(
        renderer,
        &screenWidth,
        &screenHeight
    );

    // Background
    SDL_SetRenderDrawColor(
        renderer,
        20,
        24,
        30,
        255
    );

    SDL_RenderClear(renderer);

    // -------------------------------------------------
    // Transportation networks
    // -------------------------------------------------

    SDL_SetRenderDrawColor(
        renderer,
        100,
        100,
        100,
        255
    );

    for (const auto& network :
         company.getTransportationNetworks())
    {
        if (!network->isBuilt())
        {
            continue;
        }

        const int reservoirId =
            network->getReservoirId();

        const Reservoir* reservoir = nullptr;

        for (const auto& candidate :
             company.getReservoirs())
        {
            if (candidate->getId() == reservoirId)
            {
                reservoir = candidate.get();
                break;
            }
        }

        if (reservoir == nullptr)
        {
            continue;
        }

        const Location& hqLocation =
            company.getHeadquartersLocation();

        const Location& reservoirLocation =
            reservoir->getLocation();

        SDL_FPoint hq =
            worldToScreen(
                static_cast<float>(hqLocation.getX()),
                static_cast<float>(hqLocation.getY())
            );

        SDL_FPoint field =
            worldToScreen(
                static_cast<float>(reservoirLocation.getX()),
                static_cast<float>(reservoirLocation.getY())
            );

        SDL_RenderLine(
            renderer,
            hq.x,
            hq.y,
            field.x,
            field.y
        );
    }

    // -------------------------------------------------
    // Storage facilities
    // -------------------------------------------------

    SDL_SetRenderDrawColor(
        renderer,
        80,
        180,
        120,
        255
    );

    for (const auto& storage :
         company.getStorageFacilities())
    {
        const Location& location =
            storage->getLocation();

        SDL_FPoint position =
            worldToScreen(
                static_cast<float>(location.getX()),
                static_cast<float>(location.getY())
            );

        SDL_FRect marker{
            position.x - 7.0f,
            position.y - 7.0f,
            14.0f,
            14.0f
        };

        SDL_RenderFillRect(
            renderer,
            &marker
        );
    }

    // -------------------------------------------------
    // Reservoirs
    // -------------------------------------------------

    SDL_SetRenderDrawColor(
        renderer,
        220,
        160,
        60,
        255
    );

    for (const auto& reservoir :
         company.getReservoirs())
    {
        const Location& location =
            reservoir->getLocation();

        SDL_FPoint position =
            worldToScreen(
                static_cast<float>(location.getX()),
                static_cast<float>(location.getY())
            );

        SDL_FRect marker{
            position.x - 6.0f,
            position.y - 6.0f,
            12.0f,
            12.0f
        };

        SDL_RenderFillRect(
            renderer,
            &marker
        );
    }

    // -------------------------------------------------
    // Wells
    // -------------------------------------------------

    SDL_SetRenderDrawColor(
        renderer,
        220,
        220,
        220,
        255
    );

    for (const auto& well :
         company.getWells())
    {
        const Reservoir* reservoir =
            well->getReservoir();

        if (reservoir == nullptr)
        {
            continue;
        }

        const Location& location =
            reservoir->getLocation();

        SDL_FPoint position =
            worldToScreen(
                static_cast<float>(location.getX()),
                static_cast<float>(location.getY())
            );

        SDL_FRect marker{
            position.x - 3.0f,
            position.y - 3.0f,
            6.0f,
            6.0f
        };

        SDL_RenderFillRect(
            renderer,
            &marker
        );
    }

    // -------------------------------------------------
    // Headquarters
    // -------------------------------------------------

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );

    const Location& hqLocation =
        company.getHeadquartersLocation();

    SDL_FPoint hq =
        worldToScreen(
            static_cast<float>(hqLocation.getX()),
            static_cast<float>(hqLocation.getY())
        );

    SDL_FRect hqMarker{
        hq.x - 8.0f,
        hq.y - 8.0f,
        16.0f,
        16.0f
    };

    SDL_RenderFillRect(
        renderer,
        &hqMarker
    );
    renderSelection(company);
    renderInformationPanel(company);
    SDL_RenderPresent(renderer);
}

SDL_FPoint Renderer::worldToScreen(
    float worldX,
    float worldY
) const
{
    SDL_FPoint screenPoint;

    screenPoint.x =
        (worldX - cameraX) * zoom
        + static_cast<float>(screenWidth) / 2.0f;

    screenPoint.y =
        (cameraY - worldY) * zoom
        + static_cast<float>(screenHeight) / 2.0f;

    return screenPoint;
}

void Renderer::clampZoom()
{
    if (zoom < 1.0f)
    {
        zoom = 1.0f;
    }

    if (zoom > 20.0f)
    {
        zoom = 20.0f;
    }
}

void Renderer::handleMouseWheel(float wheelY)
{
    if (wheelY > 0.0f)
    {
        zoom *= 1.15f;
    }
    else if (wheelY < 0.0f)
    {
        zoom /= 1.15f;
    }

    clampZoom();
}

void Renderer::handleKeyboard(
    const bool* keyboardState
)
{
    const float cameraSpeed = 1.0f;

    if (keyboardState[SDL_SCANCODE_W] ||
        keyboardState[SDL_SCANCODE_UP])
    {
        cameraY += cameraSpeed;
    }

    if (keyboardState[SDL_SCANCODE_S] ||
        keyboardState[SDL_SCANCODE_DOWN])
    {
        cameraY -= cameraSpeed;
    }

    if (keyboardState[SDL_SCANCODE_A] ||
        keyboardState[SDL_SCANCODE_LEFT])
    {
        cameraX -= cameraSpeed;
    }

    if (keyboardState[SDL_SCANCODE_D] ||
        keyboardState[SDL_SCANCODE_RIGHT])
    {
        cameraX += cameraSpeed;
    }

    clampCamera();
}

void Renderer::clampCamera()
{
    const float minX = -20.0f;
    const float maxX = 90.0f;

    const float minY = -20.0f;
    const float maxY = 90.0f;

    if (cameraX < minX)
    {
        cameraX = minX;
    }

    if (cameraX > maxX)
    {
        cameraX = maxX;
    }

    if (cameraY < minY)
    {
        cameraY = minY;
    }

    if (cameraY > maxY)
    {
        cameraY = maxY;
    }
}

void Renderer::handleMouseButtonDown(
    float mouseX,
    float mouseY
)
{
    isDragging = true;
    mouseMovedDuringClick = false;

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    mouseDownX = mouseX;
    mouseDownY = mouseY;
}

void Renderer::handleMouseButtonUp(
    float mouseX,
    float mouseY,
    const Company& company
)
{
    if (!mouseMovedDuringClick)
    {
        selectObjectAt(
            mouseX,
            mouseY,
            company
        );
    }

    isDragging = false;
    mouseMovedDuringClick = false;
}

void Renderer::handleMouseMotion(
    float mouseX,
    float mouseY
)
{
    if (!isDragging)
    {
        return;
    }

    const float deltaX =
        mouseX - lastMouseX;

    const float deltaY =
        mouseY - lastMouseY;

    const float totalDeltaX =
        mouseX - mouseDownX;

    const float totalDeltaY =
        mouseY - mouseDownY;

    const float dragThreshold = 5.0f;

    if (totalDeltaX > dragThreshold ||
        totalDeltaX < -dragThreshold ||
        totalDeltaY > dragThreshold ||
        totalDeltaY < -dragThreshold)
    {
        mouseMovedDuringClick = true;
    }

    cameraX -= deltaX / zoom;
    cameraY += deltaY / zoom;

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    clampCamera();
}

void Renderer::selectObjectAt(
    float mouseX,
    float mouseY,
    const Company& company
)
{
    const float selectionRadius = 12.0f;

    float bestDistanceSquared =
        selectionRadius * selectionRadius;

    SelectedObjectType bestType =
        SelectedObjectType::None;

    int bestId = -1;

    // -------------------------------------------------
    // Headquarters
    // -------------------------------------------------

    const Location& hqLocation =
        company.getHeadquartersLocation();

    SDL_FPoint hq =
        worldToScreen(
            static_cast<float>(
                hqLocation.getX()
            ),
            static_cast<float>(
                hqLocation.getY()
            )
        );

    float dx =
        mouseX - hq.x;

    float dy =
        mouseY - hq.y;

    float distanceSquared =
        dx * dx + dy * dy;

    if (distanceSquared <= bestDistanceSquared)
    {
        bestDistanceSquared = distanceSquared;

        bestType =
            SelectedObjectType::Headquarters;

        bestId = 0;
    }

    // -------------------------------------------------
    // Reservoirs
    // -------------------------------------------------

    for (const auto& reservoir :
         company.getReservoirs())
    {
        const Location& location =
            reservoir->getLocation();

        SDL_FPoint position =
            worldToScreen(
                static_cast<float>(
                    location.getX()
                ),
                static_cast<float>(
                    location.getY()
                )
            );

        dx = mouseX - position.x;
        dy = mouseY - position.y;

        distanceSquared =
            dx * dx + dy * dy;

        if (distanceSquared <= bestDistanceSquared)
        {
            bestDistanceSquared =
                distanceSquared;

            bestType =
                SelectedObjectType::Reservoir;

            bestId =
                reservoir->getId();
        }
    }

    // -------------------------------------------------
    // Wells
    // -------------------------------------------------

    for (const auto& well :
         company.getWells())
    {
        const Reservoir* reservoir =
            well->getReservoir();

        if (reservoir == nullptr)
        {
            continue;
        }

        const Location& location =
            reservoir->getLocation();

        SDL_FPoint position =
            worldToScreen(
                static_cast<float>(
                    location.getX()
                ),
                static_cast<float>(
                    location.getY()
                )
            );

        dx = mouseX - position.x;
        dy = mouseY - position.y;

        distanceSquared =
            dx * dx + dy * dy;

        if (distanceSquared <= bestDistanceSquared)
        {
            bestDistanceSquared =
                distanceSquared;

            bestType =
                SelectedObjectType::Well;

            bestId =
                well->getId();
        }
    }

    // -------------------------------------------------
    // Storage facilities
    // -------------------------------------------------

    for (const auto& storage :
         company.getStorageFacilities())
    {
        const Location& location =
            storage->getLocation();

        SDL_FPoint position =
            worldToScreen(
                static_cast<float>(
                    location.getX()
                ),
                static_cast<float>(
                    location.getY()
                )
            );

        dx = mouseX - position.x;
        dy = mouseY - position.y;

        distanceSquared =
            dx * dx + dy * dy;

        if (distanceSquared <= bestDistanceSquared)
        {
            bestDistanceSquared =
                distanceSquared;

            bestType =
                SelectedObjectType::Storage;

            bestId =
                storage->getId();
        }
    }

    // -------------------------------------------------
    // Transportation networks
    // -------------------------------------------------

    for (const auto& network :
         company.getTransportationNetworks())
    {
        if (!network->isBuilt())
        {
            continue;
        }

        const int reservoirId =
            network->getReservoirId();

        const Reservoir* reservoir =
            nullptr;

        for (const auto& candidate :
             company.getReservoirs())
        {
            if (candidate->getId() ==
                reservoirId)
            {
                reservoir =
                    candidate.get();

                break;
            }
        }

        if (reservoir == nullptr)
        {
            continue;
        }

        const Location& location =
            reservoir->getLocation();

        SDL_FPoint position =
            worldToScreen(
                static_cast<float>(
                    location.getX()
                ),
                static_cast<float>(
                    location.getY()
                )
            );

        dx = mouseX - position.x;
        dy = mouseY - position.y;

        distanceSquared =
            dx * dx + dy * dy;

        if (distanceSquared <= bestDistanceSquared)
        {
            bestDistanceSquared =
                distanceSquared;

            bestType =
                SelectedObjectType::Transportation;

            bestId =
                reservoirId;
        }
    }

    if (bestType == SelectedObjectType::None)
    {
        clearSelection();
        return;
    }
    selectedObjectType = bestType;
    selectedObjectId = bestId;
}

void Renderer::clearSelection()
{
    selectedObjectType =
        SelectedObjectType::None;

    selectedObjectId = -1;
}

void Renderer::renderSelection(
    const Company& company
)
{
    if (selectedObjectType ==
        SelectedObjectType::None)
    {
        return;
    }

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        0,
        255
    );

    SDL_FPoint position;

    if (selectedObjectType ==
        SelectedObjectType::Headquarters)
    {
        const Location& location =
            company.getHeadquartersLocation();

        position =
            worldToScreen(
                static_cast<float>(
                    location.getX()
                ),
                static_cast<float>(
                    location.getY()
                )
            );
    }
    else if (selectedObjectType ==
             SelectedObjectType::Reservoir)
    {
        for (const auto& reservoir :
             company.getReservoirs())
        {
            if (reservoir->getId() ==
                selectedObjectId)
            {
                const Location& location =
                    reservoir->getLocation();

                position =
                    worldToScreen(
                        static_cast<float>(
                            location.getX()
                        ),
                        static_cast<float>(
                            location.getY()
                        )
                    );

                SDL_FRect highlight{
                    position.x - 10.0f,
                    position.y - 10.0f,
                    20.0f,
                    20.0f
                };

                SDL_RenderRect(
                    renderer,
                    &highlight
                );

                return;
            }
        }

        return;
    }
    else if (selectedObjectType ==
             SelectedObjectType::Well)
    {
        for (const auto& well :
             company.getWells())
        {
            if (well->getId() ==
                selectedObjectId)
            {
                const Reservoir* reservoir =
                    well->getReservoir();

                if (reservoir == nullptr)
                {
                    return;
                }

                const Location& location =
                    reservoir->getLocation();

                position =
                    worldToScreen(
                        static_cast<float>(
                            location.getX()
                        ),
                        static_cast<float>(
                            location.getY()
                        )
                    );

                SDL_FRect highlight{
                    position.x - 8.0f,
                    position.y - 8.0f,
                    16.0f,
                    16.0f
                };

                SDL_RenderRect(
                    renderer,
                    &highlight
                );

                return;
            }
        }

        return;
    }
    else if (selectedObjectType ==
             SelectedObjectType::Storage)
    {
        for (const auto& storage :
             company.getStorageFacilities())
        {
            if (storage->getId() ==
                selectedObjectId)
            {
                const Location& location =
                    storage->getLocation();

                position =
                    worldToScreen(
                        static_cast<float>(
                            location.getX()
                        ),
                        static_cast<float>(
                            location.getY()
                        )
                    );

                SDL_FRect highlight{
                    position.x - 10.0f,
                    position.y - 10.0f,
                    20.0f,
                    20.0f
                };

                SDL_RenderRect(
                    renderer,
                    &highlight
                );

                return;
            }
        }

        return;
    }
    else if (selectedObjectType ==
             SelectedObjectType::Transportation)
    {
        for (const auto& reservoir :
             company.getReservoirs())
        {
            if (reservoir->getId() ==
                selectedObjectId)
            {
                const Location& location =
                    reservoir->getLocation();

                position =
                    worldToScreen(
                        static_cast<float>(
                            location.getX()
                        ),
                        static_cast<float>(
                            location.getY()
                        )
                    );

                SDL_FRect highlight{
                    position.x - 12.0f,
                    position.y - 12.0f,
                    24.0f,
                    24.0f
                };

                SDL_RenderRect(
                    renderer,
                    &highlight
                );

                return;
            }
        }

        return;
    }

    SDL_FRect highlight{
        position.x - 12.0f,
        position.y - 12.0f,
        24.0f,
        24.0f
    };

    SDL_RenderRect(
        renderer,
        &highlight
    );
}

void Renderer::renderPanelText(
    float x,
    float y,
    const char* text
)
{
    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );

    SDL_RenderDebugText(
        renderer,
        x,
        y,
        text
    );
}

void Renderer::renderInformationPanel(
    const Company& company
)
{
    if (selectedObjectType ==
        SelectedObjectType::None)
    {
        return;
    }

    const float panelX = 20.0f;
    const float panelY = 20.0f;

    const float panelWidth = 300.0f;
    const float panelHeight = 220.0f;

    SDL_FRect panel{
        panelX,
        panelY,
        panelWidth,
        panelHeight
    };

    // Panel background
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

    // Panel border
    SDL_SetRenderDrawColor(
        renderer,
        180,
        180,
        180,
        255
    );

    SDL_RenderRect(
        renderer,
        &panel
    );

    float textX = panelX + 15.0f;
    float textY = panelY + 15.0f;

    // -------------------------------------------------
    // Headquarters
    // -------------------------------------------------

    if (selectedObjectType ==
        SelectedObjectType::Headquarters)
    {
        renderPanelText(
            textX,
            textY,
            "HEADQUARTERS"
        );

        renderPanelText(
            textX,
            textY + 25.0f,
            "OilTycoon Company"
        );

        char buffer[128];

        std::snprintf(
            buffer,
            sizeof(buffer),
            "Location: %.1f, %.1f",
            company.getHeadquartersLocation().getX(),
            company.getHeadquartersLocation().getY()
        );

        renderPanelText(
            textX,
            textY + 50.0f,
            buffer
        );

        std::snprintf(
            buffer,
            sizeof(buffer),
            "Money: $%.2f",
            company.getMoney()
        );

        renderPanelText(
            textX,
            textY + 75.0f,
            buffer
        );

        std::snprintf(
            buffer,
            sizeof(buffer),
            "Reservoirs: %zu",
            company.getReservoirs().size()
        );

        renderPanelText(
            textX,
            textY + 100.0f,
            buffer
        );

        std::snprintf(
            buffer,
            sizeof(buffer),
            "Wells: %zu",
            company.getWells().size()
        );

        renderPanelText(
            textX,
            textY + 125.0f,
            buffer
        );

        std::snprintf(
            buffer,
            sizeof(buffer),
            "Storage: %zu",
            company.getStorageFacilities().size()
        );

        renderPanelText(
            textX,
            textY + 150.0f,
            buffer
        );

        return;
    }

    // -------------------------------------------------
    // Reservoir
    // -------------------------------------------------

    if (selectedObjectType ==
        SelectedObjectType::Reservoir)
    {
        for (const auto& reservoir :
             company.getReservoirs())
        {
            if (reservoir->getId() !=
                selectedObjectId)
            {
                continue;
            }

            renderPanelText(
                textX,
                textY,
                "RESERVOIR"
            );

            char buffer[128];

            std::snprintf(
                buffer,
                sizeof(buffer),
                "ID: %d",
                reservoir->getId()
            );

            renderPanelText(
                textX,
                textY + 25.0f,
                buffer
            );

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Name: %s",
                reservoir->getName().c_str()
            );

            renderPanelText(
                textX,
                textY + 50.0f,
                buffer
            );

            const Location& location =
                reservoir->getLocation();

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Location: %.1f, %.1f",
                location.getX(),
                location.getY()
            );

            renderPanelText(
                textX,
                textY + 75.0f,
                buffer
            );

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Reserves: %.0f bbl",
                reservoir->getRemainingReserves()
            );

            renderPanelText(
                textX,
                textY + 100.0f,
                buffer
            );

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Pressure: %.1f",
                reservoir->getCurrentPressure()
            );

            renderPanelText(
                textX,
                textY + 125.0f,
                buffer
            );

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Depth: %.0f m",
                reservoir->getDepth()
            );

            renderPanelText(
                textX,
                textY + 150.0f,
                buffer
            );

            return;
        }
    }

    // -------------------------------------------------
    // Well
    // -------------------------------------------------

    if (selectedObjectType ==
        SelectedObjectType::Well)
    {
        for (const auto& well :
             company.getWells())
        {
            if (well->getId() !=
                selectedObjectId)
            {
                continue;
            }

            renderPanelText(
                textX,
                textY,
                "WELL"
            );

            char buffer[128];

            std::snprintf(
                buffer,
                sizeof(buffer),
                "ID: %d",
                well->getId()
            );

            renderPanelText(
                textX,
                textY + 25.0f,
                buffer
            );

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Capability: %.1f",
                well->getMaxProductionRate()
            );

            renderPanelText(
                textX,
                textY + 50.0f,
                buffer
            );

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Production: %.1f bbl/day",
                well->calculateProduction()
            );

            renderPanelText(
                textX,
                textY + 75.0f,
                buffer
            );

            return;
        }
    }

    // -------------------------------------------------
    // Storage
    // -------------------------------------------------

    if (selectedObjectType ==
        SelectedObjectType::Storage)
    {
        for (const auto& storage :
             company.getStorageFacilities())
        {
            if (storage->getId() !=
                selectedObjectId)
            {
                continue;
            }

            renderPanelText(
                textX,
                textY,
                "STORAGE FACILITY"
            );

            char buffer[128];

            std::snprintf(
                buffer,
                sizeof(buffer),
                "ID: %d",
                storage->getId()
            );

            renderPanelText(
                textX,
                textY + 25.0f,
                buffer
            );

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Capacity: %.0f bbl",
                storage->getCapacity()
            );

            renderPanelText(
                textX,
                textY + 50.0f,
                buffer
            );

            std::snprintf(
                buffer,
                sizeof(buffer),
                "Stored: %.0f bbl",
                storage->getInventory()
            );

            renderPanelText(
                textX,
                textY + 75.0f,
                buffer
            );

            return;
        }
    }

    // -------------------------------------------------
    // Transportation
    // -------------------------------------------------

    if (selectedObjectType ==
        SelectedObjectType::Transportation)
    {
        renderPanelText(
            textX,
            textY,
            "TRANSPORTATION"
        );

        char buffer[128];

        std::snprintf(
            buffer,
            sizeof(buffer),
            "Reservoir ID: %d",
            selectedObjectId
        );

        renderPanelText(
            textX,
            textY + 25.0f,
            buffer
        );

        renderPanelText(
            textX,
            textY + 50.0f,
            "Status: Built"
        );

        return;
    }
}