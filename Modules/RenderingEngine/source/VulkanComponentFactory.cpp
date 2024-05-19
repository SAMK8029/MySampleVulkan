#include <VulkanComponentFactory.h>
#include <VulkanFunctions.h>
#include <cstring>
#include <iostream>

namespace RenderingEngine
{

VulkanComponentFactory::VulkanComponentFactory() = default;

VulkanComponentFactory::~VulkanComponentFactory() = default;

VulkanComponentFactory &VulkanComponentFactory::getInstance()
{
    static VulkanComponentFactory vulkanInitializer;

    return vulkanInitializer;
}

VkInstance VulkanComponentFactory::createVulkanInstance(const std::vector<const char*>& desiredExtensionsNames , const std::vector<const char*>& desiredLayersNames)
{
    uint32_t layersCountToCheck;
    vkEnumerateInstanceLayerProperties(&layersCountToCheck , nullptr);
    std::vector<VkLayerProperties> allAvailableLayers(layersCountToCheck);
    vkEnumerateInstanceLayerProperties(&layersCountToCheck , allAvailableLayers.data());

    if(desiredLayersNames.size() != 0)
    {
        for(const auto& desiredLayer : desiredLayersNames)
        {
            bool found = false;
            for(const auto& availableLayer : allAvailableLayers)
            {
                if(std::strcmp(availableLayer.layerName , desiredLayer) == 0)
                {
                    found = true;
                    break;
                }
            }
            if(found == false)
            {
                std::cerr << "Desired layer not found ! Layer name : " << desiredLayer << "\n\n";
                return VK_NULL_HANDLE;
            }
        }
    }

    uint32_t extensionsCountToCheck;
    vkEnumerateInstanceExtensionProperties(nullptr , &extensionsCountToCheck , nullptr);
    std::vector<VkExtensionProperties> allAvailableExtesnions(extensionsCountToCheck);
    vkEnumerateInstanceExtensionProperties(nullptr , &extensionsCountToCheck , allAvailableExtesnions.data());

    if(desiredLayersNames.size() != 0)
    {
        for(const auto& desiredLayer : desiredLayersNames)
        {
            uint32_t extensionsCountToCheck;
            vkEnumerateInstanceExtensionProperties(desiredLayer , &extensionsCountToCheck , nullptr);
            std::vector<VkExtensionProperties> layerExtesnions(extensionsCountToCheck);
            vkEnumerateInstanceExtensionProperties(desiredLayer , &extensionsCountToCheck , layerExtesnions.data());

            for(const auto& extension : layerExtesnions)
            {
                allAvailableExtesnions.push_back(extension);
            }
        }
    }

    for(const auto& desiredExtension : desiredExtensionsNames)
    {
        bool found = false;
        for(const auto& availableExtension : allAvailableExtesnions)
        {
            if(std::strcmp(availableExtension.extensionName , desiredExtension) == 0)
            {
                found = true;
                break;
            }
        }
        if(found == false)
        {
            std::cerr << "Desired extension not found ! Extension name : " << desiredExtension << "\n\n";
            return VK_NULL_HANDLE;
        }
    }

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
    instanceCreateInfo.ppEnabledLayerNames = desiredLayersNames.data();
    instanceCreateInfo.ppEnabledExtensionNames = desiredExtensionsNames.data();
    instanceCreateInfo.enabledLayerCount = desiredLayersNames.size();
    instanceCreateInfo.enabledExtensionCount = desiredExtensionsNames.size();
    instanceCreateInfo.pNext = nullptr;

    VkInstance vulkanInstance = VK_NULL_HANDLE;

    vkCreateInstance(&instanceCreateInfo , nullptr , &vulkanInstance);

    _vulkanInstance = vulkanInstance;

    return vulkanInstance;
}

VkPhysicalDevice VulkanComponentFactory::getSelectedGpu()
{

}

VkDevice VulkanComponentFactory::createVulkanLogicalDevice()
{

}

} // RenderingEngine
