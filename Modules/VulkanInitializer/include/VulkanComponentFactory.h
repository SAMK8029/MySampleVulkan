#ifndef    RENDERING_ENGINE_H
#define    RENDERING_ENGINE_H

#include <vulkan/vulkan.h>
#include <vector>

namespace RenderingEngine
{

struct QueueFamilyIndices
{
    int graphicQueueFamilyIndex;
    int presentationQueueFamilyIndex;
};


class VulkanComponentFactory
{
    VulkanComponentFactory();
    ~VulkanComponentFactory();

public:
    static VulkanComponentFactory& getInstance();

    VkDevice getCreatedVulkanLogicalDevice();
    VkDevice createVulkanLogicalDevice(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0));
    VkInstance getCreatedVulkanInstance();
    VkInstance createVulkanInstance(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0) , const std::vector<const char *>& desiredLayersNames = std::vector<const char *>(0));
    VkPhysicalDevice getSelectedGpu();
    VkPhysicalDevice getProperGpu();
    bool createSwapchian(const VkSurfaceKHR * const surface);

    QueueFamilyIndices queueFamilyIndices;

private:
    VkInstance       _vulkanInstance;
    VkDevice         _vulkanLogicalDevice;
    VkPhysicalDevice _selectedGpu;
};

} // RenderingEngine

#endif  // RENDERING_ENGINE_H
