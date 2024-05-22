#ifndef ERRPR_CODES_H
#define ERRPR_CODES_H

namespace RenderingEngine
{

enum ErrorCodes
{
    LoadingVulkanSharedLibraryFailed = 1001 ,
    NoProperGpuFound                        ,
    VulkanInstanceCreationFailed            ,
    VulkanLogicalDeviceCreationFailed       ,
    GraphicQueueNotRetrieved                ,   // 1005
    ContextWindowNotRegisterd               ,
    PresentationSurfaceNotCreated           ,
    PresentationQueueNotRetrieved           ,
    DesiredPresentationModeNotFound         ,
    DesiredIamageUsageNotSupported          ,   // 1010
    DesiredTransformNotSupported            ,
    CouldNotCreateSwapchain                 ,
    CouldNotAcquireImage                    ,
    QueuePresentError                       ,
    GlfwInitializationFailed                ,   // 1015
    CouldNotCreateWindowViaGlfw
};

} // RenderingEngine

#endif // ERRPR_CODES_H
