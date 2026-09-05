#ifndef OILTYCOON_RENDERER_H
#define OILTYCOON_RENDERER_H

#include <SDL3/SDL.h>

class Company;

enum class SelectedObjectType
{
    None,
    Headquarters,
    Reservoir,
    Well,
    Storage,
    Transportation
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void shutdown();

    void processEvents(
        bool& running,
        const Company& company
    );

    void render(const Company& company);

    SDL_FPoint worldToScreen(
        float worldX,
        float worldY
    ) const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    float cameraX;
    float cameraY;
    float zoom;

    bool isDragging;
    bool mouseMovedDuringClick;

    float lastMouseX;
    float lastMouseY;

    float mouseDownX;
    float mouseDownY;

    int screenWidth;
    int screenHeight;

    SelectedObjectType selectedObjectType;
    int selectedObjectId;

    void handleMouseWheel(float wheelY);

    void handleKeyboard(
        const bool* keyboardState
    );

    void handleMouseButtonDown(
        float mouseX,
        float mouseY
    );

    void handleMouseButtonUp(
        float mouseX,
        float mouseY,
        const Company& company
    );

    void handleMouseMotion(
        float mouseX,
        float mouseY
    );

    void selectObjectAt(
        float mouseX,
        float mouseY,
        const Company& company
    );

    void clearSelection();

    void renderSelection(
        const Company& company
    );

    void renderInformationPanel(
        const Company& company
    );

    void renderPanelText(
        float x,
        float y,
        const char* text
    );

    void clampCamera();
    void clampZoom();
};

#endif