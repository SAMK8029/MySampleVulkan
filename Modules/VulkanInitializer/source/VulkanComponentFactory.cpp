#include <VulkanComponentFactory.h>
#include <VulkanFunctions.h>
#include <cstring>
#include <iostream>

namespace RenderingEngine
{

VkInstance         VulkanComponentFactory::_vulkanInstance      = VK_NULL_HANDLE;
VkDevice           VulkanComponentFactory::_vulkanLogicalDevice = VK_NULL_HANDLE;
VkPhysicalDevice   VulkanComponentFactory::_selectedGpu         = VK_NULL_HANDLE;
QueueFamilyIndices VulkanComponentFactory::queueFamilyIndices = {-1 , -1};

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

VkPhysicalDevice VulkanComponentFactory::getProperGpu()
{
    // TODO : select the proper gpu not the first one.
    uint32_t gpusCount;
    vkEnumeratePhysicalDevices(_vulkanInstance , &gpusCount , nullptr);
    std::vector<VkPhysicalDevice> gpus(gpusCount);
    vkEnumeratePhysicalDevices(_vulkanInstance , &gpusCount , gpus.data());

    _selectedGpu = gpus.at(0);

    return gpus.at(0);
}

VkDevice VulkanComponentFactory::createVulkanLogicalDevice(const std::vector<const char*>& desiredExtensionsNames)
{
    VkPhysicalDevice selectedGpu = getProperGpu();

    uint32_t extensionsCount;
    vkEnumerateDeviceExtensionProperties(selectedGpu , nullptr , &extensionsCount , nullptr);
    std::vector<VkExtensionProperties> allAvailableExtesnions(extensionsCount);
    vkEnumerateDeviceExtensionProperties(selectedGpu , nullptr , &extensionsCount , allAvailableExtesnions.data());

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

    uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(selectedGpu , &queueFamilyPropertiesCount , nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(selectedGpu , &queueFamilyPropertiesCount , queueFamilyProperties.data());

    for(size_t i = 0 ; i < queueFamilyProperties.size() ; i++)
    {
        if(queueFamilyProperties.size() > 0 && queueFamilyProperties.at(i).queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueFamilyIndices.graphicQueueFamilyIndex = i;
            break;
        }
    }

    if(queueFamilyIndices.graphicQueueFamilyIndex == -1)
    {
        return VK_NULL_HANDLE;
    }

    float priority = 1.0;

    VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
    deviceQueueCreateInfo.queueCount = 1;
    deviceQueueCreateInfo.pQueuePriorities = &priority;
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext = nullptr;
    deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicQueueFamilyIndex;

    VkDeviceCreateInfo logicalDeviceCreateInfo{};
    logicalDeviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    logicalDeviceCreateInfo.queueCreateInfoCount = 1;
    logicalDeviceCreateInfo.pNext = nullptr;
    logicalDeviceCreateInfo.enabledExtensionCount = desiredExtensionsNames.size();
    logicalDeviceCreateInfo.ppEnabledExtensionNames = desiredExtensionsNames.data();
    logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logicalDeviceCreateInfo.pEnabledFeatures = nullptr;
    logicalDeviceCreateInfo.enabledLayerCount = 0;
    logicalDeviceCreateInfo.ppEnabledLayerNames = nullptr;

    VkDevice logicalDevice = VK_NULL_HANDLE;
    vkCreateDevice(selectedGpu , &logicalDeviceCreateInfo , nullptr , &logicalDevice);

    _vulkanLogicalDevice = logicalDevice;

    return logicalDevice;
}

VkInstance VulkanComponentFactory::getCreatedVulkanInstance()
{
    return _vulkanInstance;
}

VkDevice VulkanComponentFactory::getCreatedVulkanLogicalDevice()
{
    return _vulkanLogicalDevice;
}

VkPhysicalDevice VulkanComponentFactory::getSelectedGpu()
{
    return _selectedGpu;
}

} // RenderingEngine
