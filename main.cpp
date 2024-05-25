#include <PresentationEngine.h>
#include <VulkanFunctionsPointersInitializer.h>
#include <RenderingEngine.h>

using namespace RenderingEngine;

int main()
{
    VulkanFunctionsPointersInitializer& vulkanFunctionsPointersInitializer = VulkanFunctionsPointersInitializer::getInstance();

    vulkanFunctionsPointersInitializer.initializeVulkanFunctionsPointer();

    Render& render = Render::getInstance();
    render.draw();

    PresentationEngine& presentationEngine = PresentationEngine::getInstance();

    presentationEngine.showWindow();
}
