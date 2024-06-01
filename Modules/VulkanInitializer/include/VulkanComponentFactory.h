#ifndef    VULKAN_INITIALIZER
#define    VULKAN_INITIALIZER

#include <vulkan/vulkan.h>
#include <vector>

namespace RenderingEngine
{

class VulkanComponentFactory
{
    VulkanComponentFactory();
    ~VulkanComponentFactory();

    struct QueueFamilyIndices
    {
        int graphicQueueFamilyIndex;
        int presentationQueueFamilyIndex;
    };

public:
    static VulkanComponentFactory& getInstance();

    VkDevice getCreatedVulkanLogicalDevice() const;
    VkDevice createVulkanLogicalDevice(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0));
    VkInstance getCreatedVulkanInstance();
    VkInstance createVulkanInstance(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0) , const std::vector<const char *>& desiredLayersNames = std::vector<const char *>(0));
    VkPhysicalDevice getSelectedGpu();
    VkPhysicalDevice getProperGpu();
    VkSwapchainKHR createSwapchian(const VkSurfaceKHR * const surface);
    VkCommandPool createCommandPool();
    std::vector<VkCommandBuffer> allocateCommandBuffer();
    bool beginCommandBufferRecording();

    QueueFamilyIndices queueFamilyIndices;

    VkSurfaceFormatKHR getDesiredSurfaceFormat() const;

private:
    VkInstance       _vulkanInstance;
    VkDevice         _vulkanLogicalDevice;
    VkPhysicalDevice _selectedGpu;
    VkCommandPool    _commandPool;
    std::vector<VkCommandBuffer> _commandBuffers;
    VkSurfaceFormatKHR _desiredSurfaceFormat;
};

} // RenderingEngine

#endif  // VULKAN_INITIALIZER
