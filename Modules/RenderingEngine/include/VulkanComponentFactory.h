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

    VkInstance createVulkanInstance(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0) , const std::vector<const char *>& desiredLayersNames = std::vector<const char *>(0));
    VkPhysicalDevice getSelectedGpu();
    VkDevice createVulkanLogicalDevice();

    VkInstance _vulkanInstance;
};

} // RenderingEngine

#endif  // RENDERING_ENGINE_H
