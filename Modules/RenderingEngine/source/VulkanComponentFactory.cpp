#include <VulkanComponentFactory.h>

namespace RenderingEngine
{

VulkanComponentFactory::VulkanComponentFactory() = default;

VulkanComponentFactory::~VulkanComponentFactory() = default;

VulkanComponentFactory& VulkanComponentFactory::getInstance()
{
    static VulkanComponentFactory vulkanInitializer;

    return vulkanInitializer;
}

VkInstance VulkanComponentFactory::createVulkanInstance()
{

}

VkPhysicalDevice VulkanComponentFactory::getSelectedGpu()
{

}

VkDevice VulkanComponentFactory::createVulkanLogicalDevice()
{

}

} // RenderingEngine
