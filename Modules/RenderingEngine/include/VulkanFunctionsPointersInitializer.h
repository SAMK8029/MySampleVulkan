#ifndef VULKAN_FUNCTION_POINTERS_H
#define VULKAN_FUNCTION_POINTERS_H

#ifdef Win32
#include <Windows.h>
#elif defined Unix
#include <dlfcn.h>
#endif


namespace RenderingEngine
{

#ifdef WIN32
static HMODULE vulkanLoader = nullptr;
#elif defined Unix
static void* vulkanLoader = nullptr;
#endif

bool initializeVulkanFunctionsPointer();

} // RenderingEngine

#endif  // VULKAN_FUNCTION_POINTERS_H
