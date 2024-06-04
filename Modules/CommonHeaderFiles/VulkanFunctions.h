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
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR      vkGetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR      vkGetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;

/**************Device Level**************/

extern PFN_vkCreateCommandPool       vkCreateCommandPool;
extern PFN_vkAllocateCommandBuffers  vkAllocateCommandBuffers;
extern PFN_vkBeginCommandBuffer      vkBeginCommandBuffer;
extern PFN_vkCreateImageView         vkCreateImageView;
extern PFN_vkCreateShaderModule      vkCreateShaderModule;
extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
extern PFN_vkCreatePipelineLayout    vkCreatePipelineLayout;
extern PFN_vkCreateRenderPass        vkCreateRenderPass;
extern PFN_vkCreateFramebuffer       vkCreateFramebuffer;
extern PFN_vkCmdBeginRenderPass      vkCmdBeginRenderPass;
extern PFN_vkCmdBindPipeline         vkCmdBindPipeline;
extern PFN_vkCmdSetViewport          vkCmdSetViewport;
extern PFN_vkCmdSetScissor           vkCmdSetScissor;
extern PFN_vkCmdDraw                 vkCmdDraw;
extern PFN_vkCmdEndRenderPass        vkCmdEndRenderPass;
extern PFN_vkCreateSemaphore         vkCreateSemaphore;
extern PFN_vkCreateFence             vkCreateFence;
extern PFN_vkWaitForFences           vkWaitForFences;
extern PFN_vkResetFences             vkResetFences;

// From Extensions
extern PFN_vkCreateSwapchainKHR    vkCreateSwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
extern PFN_vkAcquireNextImageKHR   vkAcquireNextImageKHR;

/*******************************************************/

}


#endif // VULKAN_FUNCTION_H
