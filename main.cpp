#include <PresentationEngine.h>
#include <VulkanFunctionsPointersInitializer.h>
#include <RenderingEngine.h>

using namespace RenderingEngine;

int main()
{
    VulkanFunctionsPointersInitializer& vulkanFunctionsPointersInitializer = VulkanFunctionsPointersInitializer::getInstance();

    vulkanFunctionsPointersInitializer.initializeVulkanFunctionsPointer();

    PresentationEngine& presentationEngine = PresentationEngine::getInstance();
    presentationEngine.init();

    Renderer& renderer = Renderer::getInstance();
    renderer.init();

    presentationEngine.showWindow();

    return 0;
}
