#include <VulkanFunctionsPointersInitializer.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <ErrorCodes.h>

namespace RenderingEngine
{

static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

/*******Global Level*******/

static PFN_vkCreateInstance                       vkCreateInstance                      ;
static PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties    ;
static PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;

/*********************/

template<typename T>
bool checkLoadedLibrary(T library , const std::string& libraryName);

template<typename T>
bool checkLoadedFunction(T functionPointer , const std::string& functionName , const std::string& functionLevel);

bool initializeGlobalLevelVulkanFuncitons()
{
    vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(vkGetInstanceProcAddr(nullptr , "vkCreateInstance"));
    vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(vkGetInstanceProcAddr(nullptr , "vkEnumerateInstanceLayerProperties"));
    vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(vkGetInstanceProcAddr(nullptr , "vkEnumerateInstanceExtensionProperties"));

    bool result = false;

    result = checkLoadedFunction(vkCreateInstance , "vkCreateInstance" , "Global Level");
    result = checkLoadedFunction(vkEnumerateInstanceLayerProperties , "vkEnumerateInstanceLayerProperties" , "Global Level");
    result = checkLoadedFunction(vkEnumerateInstanceExtensionProperties , "vkEnumerateInstanceExtensionProperties" , "Global Level");

    return result;
}

bool initializeVulkanFunctionsPointer()
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



    return true;
}

template<typename T>
bool checkLoadedLibrary(T library , const std::string& libraryName)
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
bool checkLoadedFunction(T functionPointer , const std::string& functionName , const std::string& functionLevel)
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
