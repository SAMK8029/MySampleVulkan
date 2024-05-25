#ifndef    RENDERING_ENGINE_H
#define    RENDERING_ENGINE_H

namespace RenderingEngine
{

class Render
{

private:
    Render();
    ~Render();

public:
    static Render& getInstance();
    void draw();
};

} // RenderingEngine

#endif  // RENDERING_ENGINE_H
