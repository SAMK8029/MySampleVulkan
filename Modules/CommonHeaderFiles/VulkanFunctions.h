#ifndef  VULKAN_FUNCTION_H
#define VULKAN_FUNCTION_H

#include <vulkan/vulkan.h>

namespace RenderingEngine
{

/**************Global Level**************/

extern PFN_vkCreateInstance                       vkCreateInstance;
extern PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;

/**************Instance Level**************/

extern PFN_vkEnumerateDeviceExtensionProperties     vkEnumerateDeviceExtensionProperties;
extern PFN_vkEnumeratePhysicalDevices               vkEnumeratePhysicalDevices;
extern PFN_vkCreateDevice                           vkCreateDevice;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;

// From Extensions
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;

/**************Device Level**************/


// From Extensions
extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;

/*******************************************************/

}


#endif // VULKAN_FUNCTION_H
