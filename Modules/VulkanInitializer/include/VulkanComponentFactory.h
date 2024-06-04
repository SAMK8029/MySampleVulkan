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

    VkPhysicalDevice getProperGpu();
    VkCommandPool createCommandPool();
    VkPhysicalDevice getSelectedGpu();
    bool beginCommandBufferRecording();
    void endCommandBufferRecording();
    VkInstance getCreatedVulkanInstance();
    VkDevice getCreatedVulkanLogicalDevice() const;
    std::vector<VkCommandBuffer> allocateCommandBuffer();
    VkSwapchainKHR createSwapchian(const VkSurfaceKHR * const surface);
    VkDevice createVulkanLogicalDevice(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0));
    VkInstance createVulkanInstance(const std::vector<const char *> &desiredExtensionsNames = std::vector<const char *>(0) , const std::vector<const char *>& desiredLayersNames = std::vector<const char *>(0));
    VkQueue getDeviceQueue(uint32_t queueFamilyIndex);

    QueueFamilyIndices queueFamilyIndices;

    VkSurfaceFormatKHR getDesiredSurfaceFormat() const;

    VkSurfaceCapabilitiesKHR getSurfaceCapabilities() const;

    VkSwapchainKHR getSwapchain() const;

private:
    VkInstance                   _vulkanInstance;
    VkDevice                     _vulkanLogicalDevice;
    VkPhysicalDevice             _selectedGpu;
    VkCommandPool                _commandPool;
    std::vector<VkCommandBuffer> _commandBuffers;
    VkSurfaceFormatKHR           _desiredSurfaceFormat;
    VkSurfaceCapabilitiesKHR     _surfaceCapabilities;
    VkSwapchainKHR               _swapchain;
};

} // RenderingEngine

#endif  // VULKAN_INITIALIZER
