#include <vulkan/vulkan.h>
#include <VulkanFunctionsPointersInitializer.h>
#include <VulkanComponentFactory.h>
#include <ErrorCodes.h>
#include <iostream>
#include <vector>

namespace RenderingEngine
{

#ifndef NDEBUG  // if build in Debug mode
static const std::vector<const char*> LAYERS_NAMES {"VK_LAYER_KHRONOS_validation"};
#endif

static const std::vector<const char*> INSTANCE_EXTENSIONS_NAMES_WHICH_ARE_NOT_RELATED_TO_LAYERS{VK_KHR_SURFACE_EXTENSION_NAME , VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

static const std::vector<const char*> DEVICE_EXTENSIONS_NAMES = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};


static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;


/*******Global Level*******/

PFN_vkCreateInstance                       vkCreateInstance                      ;
PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties    ;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;

/*******Instance Level*******/

PFN_vkEnumerateDeviceExtensionProperties     vkEnumerateDeviceExtensionProperties;
PFN_vkEnumeratePhysicalDevices               vkEnumeratePhysicalDevices;
PFN_vkCreateDevice                           vkCreateDevice;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
PFN_vkGetDeviceProcAddr                      vkGetDeviceProcAddr;

// From Extensions.
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;

/*******Device Level*******/



// From Extensions.
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;

/**************************/

VulkanFunctionsPointersInitializer::VulkanFunctionsPointersInitializer() = default;

VulkanFunctionsPointersInitializer::~VulkanFunctionsPointersInitializer() = default;

bool VulkanFunctionsPointersInitializer::initializeGlobalLevelVulkanFuncitons()
{
    vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(vkGetInstanceProcAddr(nullptr , "vkCreateInstance"));
    vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(vkGetInstanceProcAddr(nullptr , "vkEnumerateInstanceLayerProperties"));
    vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(vkGetInstanceProcAddr(nullptr , "vkEnumerateInstanceExtensionProperties"));

    bool result = false;

    result = checkLoadedFunction(vkCreateInstance , "vkCreateInstance" , "Global Level");
    result = checkLoadedFunction(vkEnumerateInstanceLayerProperties , "vkEnumerateInstanceLayerProperties" , "Global Level") && result;
    result = checkLoadedFunction(vkEnumerateInstanceExtensionProperties , "vkEnumerateInstanceExtensionProperties" , "Global Level") && result;

    return result;
}

bool VulkanFunctionsPointersInitializer::initializeInstanceLevelVulkanFuncitons()
{
    vkEnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(vkGetInstanceProcAddr(VulkanComponentFactory::getCreatedVulkanInstance() , "vkEnumerateDeviceExtensionProperties"));
    vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(vkGetInstanceProcAddr(VulkanComponentFactory::getCreatedVulkanInstance() , "vkEnumeratePhysicalDevices"));
    vkCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(vkGetInstanceProcAddr(VulkanComponentFactory::getCreatedVulkanInstance() , "vkCreateDevice"));
    vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(vkGetInstanceProcAddr(VulkanComponentFactory::getCreatedVulkanInstance() , "vkGetPhysicalDeviceQueueFamilyProperties"));
    vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(vkGetInstanceProcAddr(VulkanComponentFactory::getCreatedVulkanInstance() , "vkGetDeviceProcAddr"));
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(vkGetInstanceProcAddr(VulkanComponentFactory::getCreatedVulkanInstance() , "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));

    bool result = false;
    result = checkLoadedFunction(vkEnumerateDeviceExtensionProperties , "vkEnumerateDeviceExtensionProperties" , "Instance Level");
    result = checkLoadedFunction(vkEnumeratePhysicalDevices , "vkEnumeratePhysicalDevices" , "Instance Level") && result;
    result = checkLoadedFunction(vkCreateDevice , "vkCreateDevice" , "Instance Level") && result;
    result = checkLoadedFunction(vkGetPhysicalDeviceQueueFamilyProperties , "vkGetPhysicalDeviceQueueFamilyProperties" , "Instance Level") && result;
    result = checkLoadedFunction(vkGetDeviceProcAddr , "vkGetDeviceProcAddr" , "Instance Level") && result;
    result = checkLoadedFunction(vkGetPhysicalDeviceSurfaceCapabilitiesKHR , "vkGetPhysicalDeviceSurfaceCapabilitiesKHR" , "Instance Level from extension") && result;

    return result;
}

bool VulkanFunctionsPointersInitializer::initializeDeviceLevelVulkanFuncitons()
{
    vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(vkGetDeviceProcAddr(VulkanComponentFactory::getCreatedVulkanLogicalDevice() , "vkCreateSwapchainKHR"));

    bool result = false;
    result = checkLoadedFunction(vkCreateSwapchainKHR , "vkCreateSwapchainKHR" , "Device Level from extension");

    return result;

}


VulkanFunctionsPointersInitializer& VulkanFunctionsPointersInitializer::getInstance()
{
    static VulkanFunctionsPointersInitializer vulkanFunctionsPointersInitializer;

    return vulkanFunctionsPointersInitializer;
}

bool VulkanFunctionsPointersInitializer::initializeVulkanFunctionsPointer()
{
#ifdef Win32

    vulkanLoader = LoadLibrary("vulkan-1.dll");
    vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetProcAddress(vulkanLoader , "vkGetInstanceProcAddr"));
    checkLoadedLibrary(vkGetInstanceProcAddr , "vulkan-1.dll");

#elif defined Unix

    vulkanLoader = dlopen("libvulkan.so.1" , RTLD_NOW);
    vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(vulkanLoader , "vkGetInstanceProcAddr"));
    checkLoadedLibrary(vkGetInstanceProcAddr , "vulkan-1.so.1");

#endif

    initializeGlobalLevelVulkanFuncitons();

    VkInstance vulkanInstance = VK_NULL_HANDLE;
    vulkanInstance = VulkanComponentFactory::getInstance().createVulkanInstance(INSTANCE_EXTENSIONS_NAMES_WHICH_ARE_NOT_RELATED_TO_LAYERS , LAYERS_NAMES);

    initializeInstanceLevelVulkanFuncitons();

    VkDevice logicalDevice = VK_NULL_HANDLE;
    logicalDevice = VulkanComponentFactory::getInstance().createVulkanLogicalDevice(DEVICE_EXTENSIONS_NAMES);

    initializeDeviceLevelVulkanFuncitons();

    return true;
}

template<typename T>
bool VulkanFunctionsPointersInitializer::checkLoadedLibrary(T library , const std::string& libraryName)
{
    if(library == nullptr)
    {
        std::cerr << "Could not load " << libraryName << " library" << " ! Error code : " << LoadingVulkanSharedLibraryFailed << "\n\n";

        return false;
    }
    else
    {
        std::cout << libraryName << " library loaded successfully. \n\n";

        return true;
    }
}

static unsigned int number = 1;
template<typename T>
bool VulkanFunctionsPointersInitializer::checkLoadedFunction(T functionPointer , const std::string& functionName , const std::string& functionLevel)
{
    if(functionPointer == nullptr)
    {
        std::cerr << number << " - Could not load " << functionName << " ! function. Function level : " << functionLevel << "\n\n";

        ++number;

        return false;
    }
    else
    {
        std::cout << number << " - " << functionName << " function loaded successfully. " << "Function level : " << functionLevel << "\n\n";

        ++number;

        return true;
    }
}

} // RenderingEngine