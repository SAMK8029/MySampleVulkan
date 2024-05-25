#ifndef    VULKAN_INITIALIZER
#define    VULKAN_INITIALIZER

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
    VkSwapchainKHR createSwapchian(const VkSurfaceKHR * const surface);
    VkCommandPool createCommandPool();

    QueueFamilyIndices queueFamilyIndices;

private:
    VkInstance       _vulkanInstance;
    VkDevice         _vulkanLogicalDevice;
    VkPhysicalDevice _selectedGpu;
};

} // RenderingEngine

#endif  // VULKAN_INITIALIZER
