#include "Renderer.h"

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
    }
}

void Renderer::render()
{
    SDL_GetRenderOutputSize(
    renderer,
    &screenWidth,
    &screenHeight
    );
    
    SDL_SetRenderDrawColor(
        renderer,
        20,
        24,
        30,
        255
    );

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(
        renderer,
        80,
        80,
        80,
        255
    );

    SDL_FPoint hq =
        worldToScreen(0.0f, 0.0f);

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );

    SDL_RenderPoint(
        renderer,
        hq.x,
        hq.y
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