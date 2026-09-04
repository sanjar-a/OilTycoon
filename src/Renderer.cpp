#include "Renderer.h"
#include "Company.h"
#include <iostream>

Renderer::Renderer()
    : window(nullptr),
      renderer(nullptr),
      cameraX(0.0f),
      cameraY(0.0f),
      zoom(5.0f),
      screenWidth(1280),
      screenHeight(720)
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

void Renderer::processEvents(bool& running)
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