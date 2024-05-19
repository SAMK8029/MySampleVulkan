#include <vulkan/vulkan.h>
#include <VulkanFunctionsPointersInitializer.h>
#include <VulkanComponentFactory.h>
#include <ErrorCodes.h>
#include <iostream>
#include <vector>

namespace RenderingEngine
{

#ifndef NDEBUG  // if Debug
static const std::vector<const char*> VALIDATION_NAMES {"VK_LAYER_KHRONOS_validation"};
#endif

static const std::vector<const char*> INSTANCE_EXTENSIONS_NAMES_WHICH_ARE_NOT_RELATED_TO_LAYERS;


static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;


/*******Global Level*******/

PFN_vkCreateInstance                       vkCreateInstance                      ;
PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties    ;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;

/*********************/

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

VulkanFunctionsPointersInitializer::VulkanFunctionsPointersInitializer() = default;

VulkanFunctionsPointersInitializer::~VulkanFunctionsPointersInitializer() = default;

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

    vulkanInstance = VulkanComponentFactory::getInstance().createVulkanInstance(INSTANCE_EXTENSIONS_NAMES_WHICH_ARE_NOT_RELATED_TO_LAYERS , VALIDATION_NAMES);

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

static int number = 1;
template<typename T>
bool VulkanFunctionsPointersInitializer::checkLoadedFunction(T functionPointer , const std::string& functionName , const std::string& functionLevel)
{
    if(functionPointer == nullptr)
    {
        std::cerr << number << " - Could not load " << functionName << " ! function. Function level : " << functionLevel << "\n\n";

        return false;
    }
    else
    {
        std::cout << number << " - " << functionName << " function loaded successfully. " << "Function level : " << functionLevel << "\n\n";

        return true;
    }

    ++number;
}


} // RenderingEngine
