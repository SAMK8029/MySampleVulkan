#include <VulkanComponentFactory.h>
#include <VulkanFunctions.h>
#include <cstring>
#include <iostream>

namespace RenderingEngine
{

VulkanComponentFactory::VulkanComponentFactory()
{
    _vulkanInstance      = VK_NULL_HANDLE;
    _vulkanLogicalDevice = VK_NULL_HANDLE;
    _selectedGpu         = VK_NULL_HANDLE;
    queueFamilyIndices = {-1 , -1};
}

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

VkSwapchainKHR VulkanComponentFactory::createSwapchian(const VkSurfaceKHR* const surface)
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(getSelectedGpu() , *surface , &surfaceCapabilities);

    uint32_t desiredMinimumImage = surfaceCapabilities.minImageCount + 1;

    if(surfaceCapabilities.maxImageCount > 0 && desiredMinimumImage > surfaceCapabilities.maxImageCount)
    {
        desiredMinimumImage = surfaceCapabilities.maxImageCount;
    }

    uint32_t surfaceFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(getSelectedGpu() , *surface , &surfaceFormatCount , nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(getSelectedGpu() , *surface , &surfaceFormatCount , surfaceFormats.data());

    VkSurfaceFormatKHR desiredSurfaceFormat{VkFormat::VK_FORMAT_R8G8B8A8_UNORM , VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

    {
        bool found = false;
        for(const auto& surfaceFormat : surfaceFormats)
        {
            if(surfaceFormat.format == desiredSurfaceFormat.format && surfaceFormat.colorSpace == desiredSurfaceFormat.colorSpace)
            {
                found = true;
                break;
            }
        }

        if(found == false)
        {
            desiredSurfaceFormat = surfaceFormats.at(0);
        }
    }

    uint32_t queueFamiliesPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(getSelectedGpu() , &queueFamiliesPropertiesCount , nullptr);
    std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamiliesPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(getSelectedGpu() , &queueFamiliesPropertiesCount , queueFamiliesProperties.data());


    // TODO : Make sure that the presentation queue and graphic queue are identical if posible by checking all queue family properties.
    for(size_t i = 0 ; i < queueFamiliesProperties.size() ; i++)
    {
        VkBool32 isSupported = VK_FALSE;
        VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(getSelectedGpu() , i , *surface , &isSupported);

        if(isSupported == VK_TRUE && result == VK_SUCCESS)
        {
            queueFamilyIndices.presentationQueueFamilyIndex = i;
            break;
        }
    }

    uint32_t presentationModesCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(getSelectedGpu() , *surface , &presentationModesCount , nullptr);
    std::vector<VkPresentModeKHR> presentationModes(presentationModesCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(getSelectedGpu() , *surface , &presentationModesCount , presentationModes.data());

    VkPresentModeKHR desiredPresentationMode = VK_PRESENT_MODE_MAILBOX_KHR;
    {
        bool found = false;
        for(const auto& presentationMode : presentationModes)
        {
            if(desiredPresentationMode == presentationMode)
            {
                found = true;
                break;
            }
        }

        if(found == false)
        {
            desiredPresentationMode = presentationModes.at(1);
        }
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.surface = *surface;
    swapchainCreateInfo.clipped = true;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.oldSwapchain = nullptr;
    swapchainCreateInfo.minImageCount = desiredMinimumImage;
    swapchainCreateInfo.imageFormat = desiredSurfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = desiredSurfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
    swapchainCreateInfo.imageArrayLayers = 1; // ?
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = desiredPresentationMode;

    if(queueFamilyIndices.presentationQueueFamilyIndex == queueFamilyIndices.graphicQueueFamilyIndex)
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Excusive to the queue.
        swapchainCreateInfo.queueFamilyIndexCount = 0;  // ~
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;  // ~
    }
    else
    {
        uint32_t indices[] = {static_cast<uint32_t>(queueFamilyIndices.graphicQueueFamilyIndex) , static_cast<uint32_t>(queueFamilyIndices.presentationQueueFamilyIndex)};

        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = indices;
    }

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    vkCreateSwapchainKHR(_vulkanLogicalDevice , &swapchainCreateInfo , nullptr , &swapchain);

    return swapchain;
}

VkCommandPool VulkanComponentFactory::createCommandPool()
{
    VkCommandPoolCreateInfo CommandPoolCreateInfo{};
    CommandPoolCreateInfo.pNext = nullptr;
    CommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    CommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    CommandPoolCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicQueueFamilyIndex;

    VkCommandPool commandPool = VK_NULL_HANDLE;
    vkCreateCommandPool(_vulkanLogicalDevice , &CommandPoolCreateInfo , nullptr , &commandPool);

    _commandPool = commandPool;

    return commandPool;
}

std::vector<VkCommandBuffer> VulkanComponentFactory::allocateCommandBuffer()
{
    uint32_t count = 10;

    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
    commandBufferAllocateInfo.commandPool = _commandPool;
    commandBufferAllocateInfo.pNext = nullptr;
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = count;

    std::vector<VkCommandBuffer> commandBuffers(count);

    vkAllocateCommandBuffers(_vulkanLogicalDevice , &commandBufferAllocateInfo ,  commandBuffers.data());

    return commandBuffers;
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
