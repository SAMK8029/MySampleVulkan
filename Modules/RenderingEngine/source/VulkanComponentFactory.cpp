#include <VulkanComponentFactory.h>
#include <VulkanFunctions.h>
#include <cstring>

namespace RenderingEngine
{

VulkanComponentFactory::VulkanComponentFactory() = default;

VulkanComponentFactory::~VulkanComponentFactory() = default;

VulkanComponentFactory &VulkanComponentFactory::getInstance()
{
    static VulkanComponentFactory vulkanInitializer;

    return vulkanInitializer;
}

VkInstance VulkanComponentFactory::createVulkanInstance(const std::vector<const char*>& extensionsNames , uint32_t extensionsCount , const std::vector<const char*>& desiredLayersNames , uint32_t layersCount)
{
    uint32_t layersCountToCheck;
    vkEnumerateInstanceLayerProperties(&layersCountToCheck , nullptr);
    std::vector<VkLayerProperties> allAvailableLayers(layersCountToCheck);
    vkEnumerateInstanceLayerProperties(&layersCountToCheck , allAvailableLayers.data());

    // TODO : CHECK AVAILABLE LAYERS AND EXTENSIONS.

    VkApplicationInfo applicationInfo{};
    applicationInfo.applicationVersion = 1;
    applicationInfo.apiVersion = VK_API_VERSION_1_3;
    applicationInfo.engineVersion = 1;
    applicationInfo.pNext = nullptr;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pEngineName = "MySampleVulkan";
    applicationInfo.pApplicationName = "MySampleVulkan";

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.ppEnabledLayerNames = desiredLayersNames;
    instanceCreateInfo.ppEnabledExtensionNames = extensionsNames;
    instanceCreateInfo.enabledLayerCount = layersCount;
    instanceCreateInfo.enabledExtensionCount = extensionsCount;
    instanceCreateInfo.pNext = nullptr;

    VkInstance vulkanInstance = VK_NULL_HANDLE;
}

VkPhysicalDevice VulkanComponentFactory::getSelectedGpu()
{

}

VkDevice VulkanComponentFactory::createVulkanLogicalDevice()
{

}

} // RenderingEngine
