#ifndef OILTYCOON_RENDERER_H
#define OILTYCOON_RENDERER_H

#include <SDL3/SDL.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void shutdown();

    void processEvents(bool& running);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif