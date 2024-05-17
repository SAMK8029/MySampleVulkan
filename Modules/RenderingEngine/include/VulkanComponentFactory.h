#ifndef    RENDERING_ENGINE_H
#define    RENDERING_ENGINE_H

#include <vulkan/vulkan.h>

namespace RenderingEngine
{

class VulkanComponentFactory
{
    VulkanComponentFactory();
    ~VulkanComponentFactory();

public:
    VulkanComponentFactory &getInstance();

    VkInstance createVulkanInstance();
    VkPhysicalDevice getSelectedGpu();
    VkDevice createVulkanLogicalDevice();
};

} // RenderingEngine

#endif  // RENDERING_ENGINE_H
