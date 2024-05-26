#include <VulkanComponentFactory.h>
#include <RenderingEngine.h>

namespace RenderingEngine
{

Render::Render() = default;

Render::~Render() = default;

Render& Render::getInstance()
{
    static Render renderingEngine;

    return renderingEngine;
}

void Render::draw()
{
    VulkanComponentFactory& vulkanComponentFactory = VulkanComponentFactory::getInstance();

    vulkanComponentFactory.createCommandPool();
    vulkanComponentFactory.allocateCommandBuffer();
}

} // RenderingEngine
