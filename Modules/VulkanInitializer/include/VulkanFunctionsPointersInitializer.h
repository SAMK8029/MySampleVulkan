#ifndef VULKAN_FUNCTION_POINTERS_H
#define VULKAN_FUNCTION_POINTERS_H

#ifdef Win32
#include <Windows.h>
#elif defined Unix
#include <dlfcn.h>
#endif

#include <string>

namespace RenderingEngine
{

class VulkanFunctionsPointersInitializer
{
    VulkanFunctionsPointersInitializer();
    ~VulkanFunctionsPointersInitializer();

public:
    static VulkanFunctionsPointersInitializer& getInstance();

    bool initializeVulkanFunctionsPointer();

private:
    bool initializeGlobalLevelVulkanFuncitons();
    bool initializeInstanceLevelVulkanFuncitons();
    bool initializeDeviceLevelVulkanFuncitons();

    template<typename T>
    bool checkLoadedLibrary(T library , const std::string& libraryName);

    template<typename T>
    bool checkLoadedFunction(T functionPointer , const std::string& functionName , const std::string& functionLevel);

#ifdef WIN32
    HMODULE vulkanLoader = nullptr;
#elif defined Unix
    void* vulkanLoader = nullptr;
#endif

};

} // RenderingEngine

#endif  // VULKAN_FUNCTION_POINTERS_H
