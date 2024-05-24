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
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    glfwCreateWindowSurface(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , _window , nullptr , &surface);

    VulkanComponentFactory::getInstance().createSwapchian(&surface);

    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
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

PresentationEngine::~PresentationEngine() = default;

} // RenderingEngine
