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
#else
static const std::vector<const char*> LAYERS_NAMES{};
#endif

static const std::vector<const char*> INSTANCE_EXTENSIONS_NAMES_WHICH_ARE_NOT_RELATED_TO_LAYERS
    {
        VK_KHR_SURFACE_EXTENSION_NAME ,
#ifdef Win32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined Unix
        VK_KHR_XCB_SURFACE_EXTENSION_NAME
#endif
    };

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
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR      vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR      vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;

/*******Device Level*******/

PFN_vkCreateCommandPool       vkCreateCommandPool;
PFN_vkAllocateCommandBuffers  vkAllocateCommandBuffers;
PFN_vkBeginCommandBuffer      vkBeginCommandBuffer;
PFN_vkCreateImageView         vkCreateImageView;
PFN_vkCreateShaderModule      vkCreateShaderModule;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
PFN_vkCreatePipelineLayout    vkCreatePipelineLayout;
PFN_vkCreateRenderPass        vkCreateRenderPass;
PFN_vkCreateFramebuffer       vkCreateFramebuffer;
PFN_vkCmdBeginRenderPass      vkCmdBeginRenderPass;
PFN_vkCmdBindPipeline         vkCmdBindPipeline;
PFN_vkCmdSetViewport          vkCmdSetViewport;
PFN_vkCmdSetScissor           vkCmdSetScissor;
PFN_vkCmdDraw                 vkCmdDraw;
PFN_vkCmdEndRenderPass        vkCmdEndRenderPass;
PFN_vkCreateSemaphore         vkCreateSemaphore;
PFN_vkCreateFence             vkCreateFence;
PFN_vkWaitForFences           vkWaitForFences;
PFN_vkResetFences             vkResetFences;

// From Extensions.
PFN_vkCreateSwapchainKHR    vkCreateSwapchainKHR;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
PFN_vkAcquireNextImageKHR   vkAcquireNextImageKHR;

/**************************/

VulkanFunctionsPointersInitializer:: VulkanFunctionsPointersInitializer() = default;
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
    vkEnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkEnumerateDeviceExtensionProperties"));
    vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkEnumeratePhysicalDevices"));
    vkCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkCreateDevice"));
    vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkGetPhysicalDeviceQueueFamilyProperties"));
    vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkGetDeviceProcAddr"));
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
    vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkGetPhysicalDeviceSurfaceFormatsKHR"));
    vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkGetPhysicalDeviceSurfaceSupportKHR"));
    vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(vkGetInstanceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanInstance() , "vkGetPhysicalDeviceSurfacePresentModesKHR"));

    bool result = false;
    result = checkLoadedFunction(vkEnumerateDeviceExtensionProperties , "vkEnumerateDeviceExtensionProperties" , "Instance Level");
    result = checkLoadedFunction(vkEnumeratePhysicalDevices , "vkEnumeratePhysicalDevices" , "Instance Level") && result;
    result = checkLoadedFunction(vkCreateDevice , "vkCreateDevice" , "Instance Level") && result;
    result = checkLoadedFunction(vkGetPhysicalDeviceQueueFamilyProperties , "vkGetPhysicalDeviceQueueFamilyProperties" , "Instance Level") && result;
    result = checkLoadedFunction(vkGetDeviceProcAddr , "vkGetDeviceProcAddr" , "Instance Level") && result;
    result = checkLoadedFunction(vkGetPhysicalDeviceSurfaceCapabilitiesKHR , "vkGetPhysicalDeviceSurfaceCapabilitiesKHR" , "Instance Level from extension") && result;
    result = checkLoadedFunction(vkGetPhysicalDeviceSurfaceFormatsKHR , "vkGetPhysicalDeviceSurfaceFormatsKHR" , "Instance Level from extension") && result;
    result = checkLoadedFunction(vkGetPhysicalDeviceSurfaceSupportKHR , "vkGetPhysicalDeviceSurfaceSupportKHR" , "Instance Level from extension") && result;
    result = checkLoadedFunction(vkGetPhysicalDeviceSurfacePresentModesKHR , "vkGetPhysicalDeviceSurfacePresentModesKHR" , "Instance Level from extension") && result;

    return result;
}

bool VulkanFunctionsPointersInitializer::initializeDeviceLevelVulkanFuncitons()
{
    vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateSwapchainKHR"));
    vkCreateCommandPool = reinterpret_cast<PFN_vkCreateCommandPool>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateCommandPool"));
    vkAllocateCommandBuffers = reinterpret_cast<PFN_vkAllocateCommandBuffers>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkAllocateCommandBuffers"));
    vkBeginCommandBuffer = reinterpret_cast<PFN_vkBeginCommandBuffer>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkBeginCommandBuffer"));
    vkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkGetSwapchainImagesKHR"));
    vkCreateImageView = reinterpret_cast<PFN_vkCreateImageView>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateImageView"));
    vkCreateShaderModule = reinterpret_cast<PFN_vkCreateShaderModule>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateShaderModule"));
    vkCreateGraphicsPipelines = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateGraphicsPipelines"));
    vkCreatePipelineLayout = reinterpret_cast<PFN_vkCreatePipelineLayout>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreatePipelineLayout"));
    vkCreateRenderPass = reinterpret_cast<PFN_vkCreateRenderPass>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateRenderPass"));
    vkCreateFramebuffer = reinterpret_cast<PFN_vkCreateFramebuffer>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateFramebuffer"));
    vkCmdBeginRenderPass = reinterpret_cast<PFN_vkCmdBeginRenderPass>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCmdBeginRenderPass"));
    vkCmdBindPipeline = reinterpret_cast<PFN_vkCmdBindPipeline>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCmdBindPipeline"));
    vkCmdSetViewport = reinterpret_cast<PFN_vkCmdSetViewport>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCmdSetViewport"));
    vkCmdSetScissor = reinterpret_cast<PFN_vkCmdSetScissor>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCmdSetScissor"));
    vkCmdDraw = reinterpret_cast<PFN_vkCmdDraw>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCmdDraw"));
    vkCmdEndRenderPass = reinterpret_cast<PFN_vkCmdEndRenderPass>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCmdEndRenderPass"));
    vkCreateSemaphore = reinterpret_cast<PFN_vkCreateSemaphore>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateSemaphore"));
    vkCreateFence = reinterpret_cast<PFN_vkCreateFence>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkCreateFence"));
    vkWaitForFences = reinterpret_cast<PFN_vkWaitForFences>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkWaitForFences"));
    vkResetFences = reinterpret_cast<PFN_vkResetFences>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkResetFences"));
    vkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(vkGetDeviceProcAddr(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , "vkAcquireNextImageKHR"));

    bool result = false;
    result = checkLoadedFunction(vkCreateSwapchainKHR , "vkCreateSwapchainKHR" , "Device Level from extension");
    result = checkLoadedFunction(vkCreateCommandPool , "vkCreateCommandPool" , "Device Level") && result;
    result = checkLoadedFunction(vkAllocateCommandBuffers , "vkAllocateCommandBuffers" , "Device Level") && result;
    result = checkLoadedFunction(vkBeginCommandBuffer , "vkBeginCommandBuffer" , "Device Level") && result;
    result = checkLoadedFunction(vkGetSwapchainImagesKHR , "vkGetSwapchainImagesKHR" , "Device Level from extension") && result;
    result = checkLoadedFunction(vkCreateImageView , "vkCreateImageView" , "Device Level") && result;
    result = checkLoadedFunction(vkCreateShaderModule , "vkCreateShaderModule" , "Device Level") && result;
    result = checkLoadedFunction(vkCreateGraphicsPipelines , "vkCreateGraphicsPipelines" , "Device Level") && result;
    result = checkLoadedFunction(vkCreatePipelineLayout , "vkCreatePipelineLayout" , "Device Level") && result;
    result = checkLoadedFunction(vkCreateRenderPass , "vkCreateRenderPass" , "Device Level") && result;
    result = checkLoadedFunction(vkCreateFramebuffer , "vkCreateFramebuffer" , "Device Level") && result;
    result = checkLoadedFunction(vkCmdBeginRenderPass , "vkCmdBeginRenderPass" , "Device Level") && result;
    result = checkLoadedFunction(vkCmdBindPipeline , "vkCmdBindPipeline" , "Device Level") && result;
    result = checkLoadedFunction(vkCmdSetViewport , "vkCmdSetViewport" , "Device Level") && result;
    result = checkLoadedFunction(vkCmdSetScissor , "vkCmdSetScissor" , "Device Level") && result;
    result = checkLoadedFunction(vkCmdDraw , "vkCmdDraw" , "Device Level") && result;
    result = checkLoadedFunction(vkCmdEndRenderPass , "vkCmdEndRenderPass" , "Device Level") && result;
    result = checkLoadedFunction(vkCreateSemaphore , "vkCreateSemaphore" , "Device Level") && result;
    result = checkLoadedFunction(vkCreateFence , "vkCreateFence" , "Device Level") && result;
    result = checkLoadedFunction(vkWaitForFences , "vkWaitForFences" , "Device Level") && result;
    result = checkLoadedFunction(vkResetFences , "vkResetFences" , "Device Level") && result;
    result = checkLoadedFunction(vkAcquireNextImageKHR , "vkAcquireNextImageKHR" , "Device Level from extension") && result;

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
        std::cout << number << " - Could not load " << functionName << " ! function. Function level : " << functionLevel << "\n\n";

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
