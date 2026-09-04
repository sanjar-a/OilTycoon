#ifndef OILTYCOON_RENDERER_H
#define OILTYCOON_RENDERER_H

#include <SDL3/SDL.h>

class Company;
class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void shutdown();

    void processEvents(bool& running);
    void render(const Company& company);

    SDL_FPoint worldToScreen(float worldX, float worldY) const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    float cameraX;
    float cameraY;
    float zoom;

    bool isDragging;
    float lastMouseX;
    float lastMouseY;

    int screenWidth;
    int screenHeight;

    void handleMouseWheel(float wheelY);
    void handleKeyboard(const bool* keyboardState);
    void handleMouseButtonDown(float mouseX, float mouseY);
    void handleMouseButtonUp();
    void handleMouseMotion(float mouseX, float mouseY);
    void clampCamera();
    void clampZoom();
};

#endif