#include <VulkanComponentFactory.h>
#include <PresentationEngine.h>
#include <VulkanFunctions.h>
#include <ErrorCodes.h>
#include <iostream>

namespace RenderingEngine
{

PresentationEngine& PresentationEngine::getInstance()
{
    static PresentationEngine presentationEngine;

    return presentationEngine;
}

void PresentationEngine::showWinodw()
{
    createSwapchian();

    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
}

PresentationEngine:: PresentationEngine()
{
    if(!glfwInit())
    {
        std::cerr << "Glfw In`itialization Failed ! \n\n";
        std::exit(GlfwInitializationFailed);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(960 , 540 , "MySampleVulkan" , nullptr , nullptr);

    if(_window == nullptr)
    {
        std::cerr << "Could Not Create Window Via Glfw! \n\n";
        std::exit(CouldNotCreateWindowViaGlfw);
    }

    glfwMakeContextCurrent(_window);
}
PresentationEngine::~PresentationEngine() = default;


bool PresentationEngine::createSwapchian()
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    glfwCreateWindowSurface(VulkanComponentFactory::getCreatedVulkanInstance() , _window , nullptr , &surface);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanComponentFactory::getSelectedGpu() , surface , &surfaceCapabilities);

    uint32_t desiredMinimumImage = surfaceCapabilities.minImageCount + 1;

    if(surfaceCapabilities.maxImageCount > 0 && desiredMinimumImage > surfaceCapabilities.maxImageCount)
    {
        desiredMinimumImage = surfaceCapabilities.maxImageCount;
    }

    uint32_t surfaceFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanComponentFactory::getSelectedGpu() , surface , &surfaceFormatCount , nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanComponentFactory::getSelectedGpu() , surface , &surfaceFormatCount , surfaceFormats.data());

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
    vkGetPhysicalDeviceQueueFamilyProperties(VulkanComponentFactory::getSelectedGpu() , &queueFamiliesPropertiesCount , nullptr);
    std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamiliesPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(VulkanComponentFactory::getSelectedGpu() , &queueFamiliesPropertiesCount , queueFamiliesProperties.data());


    // TODO : Make sure that the presentation queue and graphic queue are identical if posible by checking all queue family properties.
    for(size_t i = 0 ; i < queueFamiliesProperties.size() ; i++)
    {
        VkBool32 isSupported = VK_FALSE;
        VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(VulkanComponentFactory::getSelectedGpu() , i , surface , &isSupported);

        if(isSupported == VK_TRUE && result == VK_SUCCESS)
        {
            VulkanComponentFactory::queueFamilyIndices.presentationQueueFamilyIndex = i;
            break;
        }
    }

    uint32_t presentationModesCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanComponentFactory::getSelectedGpu() , surface , &presentationModesCount , nullptr);
    std::vector<VkPresentModeKHR> presentationModes(presentationModesCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanComponentFactory::getSelectedGpu() , surface , &presentationModesCount , presentationModes.data());

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

    VkSwapchainCreateInfoKHR SwapchainCreateInfo{};
    SwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    SwapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    SwapchainCreateInfo.surface = surface;
    SwapchainCreateInfo.clipped = true;
    SwapchainCreateInfo.pNext = nullptr;
    SwapchainCreateInfo.oldSwapchain = nullptr;
    SwapchainCreateInfo.minImageCount = desiredMinimumImage;
    SwapchainCreateInfo.imageFormat = desiredSurfaceFormat.format;
    SwapchainCreateInfo.imageColorSpace = desiredSurfaceFormat.colorSpace;
    SwapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
    SwapchainCreateInfo.imageArrayLayers = 1; // ?
    SwapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    SwapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    SwapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    SwapchainCreateInfo.presentMode = desiredPresentationMode;

    if(VulkanComponentFactory::queueFamilyIndices.presentationQueueFamilyIndex == VulkanComponentFactory::queueFamilyIndices.graphicQueueFamilyIndex)
    {
        SwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Excusive to the queue.
        SwapchainCreateInfo.queueFamilyIndexCount = 0;  // ~
        SwapchainCreateInfo.pQueueFamilyIndices = nullptr;  // ~
    }
    else
    {
        uint32_t queueFamilyIndices[] = {static_cast<uint32_t>(VulkanComponentFactory::queueFamilyIndices.graphicQueueFamilyIndex) , static_cast<uint32_t>(VulkanComponentFactory::queueFamilyIndices.presentationQueueFamilyIndex)};

        SwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        SwapchainCreateInfo.queueFamilyIndexCount = 2;
        SwapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    vkCreateSwapchainKHR(VulkanComponentFactory::getCreatedVulkanLogicalDevice() , &SwapchainCreateInfo , nullptr , &swapchain);

    return true;
}

} // RenderingEngine
