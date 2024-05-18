#ifndef    RENDERING_ENGINE_H
#define    RENDERING_ENGINE_H

#include <vulkan/vulkan.h>
#include <vector>

namespace RenderingEngine
{

class VulkanComponentFactory
{
    VulkanComponentFactory();
    ~VulkanComponentFactory();

public:
    static VulkanComponentFactory &getInstance();

    VkInstance createVulkanInstance(const std::vector<const char *> &extensionsNames , uint32_t extensionsCount , const std::vector<const char *> &desiredLayersNames = nullptr , uint32_t layersCount = 0);
    VkPhysicalDevice getSelectedGpu();
    VkDevice createVulkanLogicalDevice();
};

} // RenderingEngine

#endif  // RENDERING_ENGINE_H
