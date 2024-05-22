#ifndef PRESENTATION_ENGINE_H
#define PRESENTATION_ENGINE_H

#include <glfw3.h>

namespace RenderingEngine
{

class PresentationEngine
{
public:
    static PresentationEngine& getInstance();
    void showWinodw();

private:
    PresentationEngine();
    ~PresentationEngine();

    bool createSwapchian();

    GLFWwindow*      _window;
};

} // RenderingEngine

#endif // PRESENTATION_ENGINE_H
