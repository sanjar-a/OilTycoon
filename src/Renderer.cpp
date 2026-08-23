#include "Renderer.h"

#include <iostream>

Renderer::Renderer()
    : window(nullptr),
      renderer(nullptr)
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