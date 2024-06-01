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

void PresentationEngine::showWindow()
{
    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
}

void PresentationEngine::init()
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    glfwCreateWindowSurface(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , _window , nullptr , &surface);

    _swapchain = VulkanComponentFactory::getInstance().createSwapchian(&surface);

    getSwapchainImages(&_swapchainImages);

    createImageViews(&_imageViews);
}

PresentationEngine:: PresentationEngine()
{
    if(!glfwInit())
    {
        std::cerr << "Glfw Initialization Failed ! \n\n";
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

void PresentationEngine::getSwapchainImages(std::vector<VkImage>* swapchainImages)
{
    VulkanComponentFactory& vulkanComponentFactory = VulkanComponentFactory::getInstance();

    swapchainImages->clear();

    uint32_t swapchainImagesCount;
    vkGetSwapchainImagesKHR(vulkanComponentFactory.getCreatedVulkanLogicalDevice() , _swapchain , &swapchainImagesCount , nullptr);
    swapchainImages->resize(swapchainImagesCount);
    vkGetSwapchainImagesKHR(vulkanComponentFactory.getCreatedVulkanLogicalDevice() , _swapchain , &swapchainImagesCount , _swapchainImages.data());
}

void PresentationEngine::createImageViews(std::vector<VkImageView>* imageViews)
{
    imageViews->clear();

    imageViews->resize(_swapchainImages.size());

    for(size_t i = 0 ; i < imageViews->size() ; ++i)
    {
        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.image = _swapchainImages.at(i);
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.format = VulkanComponentFactory::getInstance().getDesiredSurfaceFormat().format;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;

        vkCreateImageView(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , &imageViewCreateInfo , nullptr , &imageViews->at(i));
    }
}

PresentationEngine::~PresentationEngine() = default;

} // RenderingEngine
