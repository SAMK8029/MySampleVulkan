#ifndef  VULKAN_FUNCTION_H
#define VULKAN_FUNCTION_H

#include <vulkan/vulkan.h>

namespace RenderingEngine
{

/**************Global Level Functions**************/

extern PFN_vkCreateInstance                       vkCreateInstance;
extern PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;

/**************************************************/

}


#endif // VULKAN_FUNCTION_H
