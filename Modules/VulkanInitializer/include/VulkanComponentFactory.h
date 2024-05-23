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
    static VulkanComponentFactory &getInstance();
    static VkInstance getCreatedVulkanInstance();
    static VkDevice   getCreatedVulkanLogicalDevice();
    static VkPhysicalDevice getSelectedGpu();
    static QueueFamilyIndices queueFamilyIndices;

    VkInstance createVulkanInstance(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0) , const std::vector<const char *>& desiredLayersNames = std::vector<const char *>(0));
    VkPhysicalDevice getProperGpu();
    VkDevice createVulkanLogicalDevice(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0));


private:
    static VkInstance _vulkanInstance;
    static VkDevice   _vulkanLogicalDevice;
    static VkPhysicalDevice   _selectedGpu;
};

} // RenderingEngine

#endif  // RENDERING_ENGINE_H
