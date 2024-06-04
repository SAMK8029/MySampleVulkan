#include <VulkanComponentFactory.h>
#include <RenderingEngine.h>
#include <VulkanFunctions.h>
#include <PresentationEngine.h>
#include <fstream>

namespace RenderingEngine
{

Renderer::Renderer()
{
    std::ifstream fragmentShaderFile("Shaders/FragmentShader.spv" , std::ios::ate | std::ios::binary);
    const size_t fragmentShaderFileSize = static_cast<const size_t>(fragmentShaderFile.tellg());
    _fragmentShader.resize(fragmentShaderFileSize);
    fragmentShaderFile.seekg(0);
    fragmentShaderFile.read(_fragmentShader.data() , fragmentShaderFileSize);
    fragmentShaderFile.close();

    std::ifstream vertexShaderFile("Shaders/VertexShader.spv" , std::ios::ate | std::ios::binary);
    const size_t vertexShaderFileSize = static_cast<const size_t>(vertexShaderFile.tellg());
    _vertexShader.resize(vertexShaderFileSize);
    vertexShaderFile.seekg(0);
    vertexShaderFile.read(_vertexShader.data() , vertexShaderFileSize);
    vertexShaderFile.close();
}

Renderer::~Renderer() = default;

Renderer& Renderer::getInstance()
{
    static Renderer renderingEngine;

    return renderingEngine;
}

void RenderingEngine::Renderer::recordCommandBuufer(VkRenderPass renderPass , uint32_t imageIndex)
{
    VulkanComponentFactory& vulkanComponentFactory = VulkanComponentFactory::getInstance();

    vulkanComponentFactory.beginCommandBufferRecording();

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = _framebuffers[imageIndex];
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = vulkanComponentFactory.getSurfaceCapabilities().currentExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(_commandBuffers.at(0) , &renderPassBeginInfo , VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(_commandBuffers.at(0) , VK_PIPELINE_BIND_POINT_GRAPHICS , _graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(vulkanComponentFactory.getSurfaceCapabilities().currentExtent.width);
    viewport.height = static_cast<float>(vulkanComponentFactory.getSurfaceCapabilities().currentExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(_commandBuffers.at(0), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = vulkanComponentFactory.getSurfaceCapabilities().currentExtent;
    vkCmdSetScissor(_commandBuffers.at(0), 0, 1, &scissor);

    vkCmdDraw(_commandBuffers.at(0), 3, 1, 0, 0);

    vkCmdEndRenderPass(_commandBuffers.at(0));

    vulkanComponentFactory.endCommandBufferRecording();
}

void Renderer::init()
{
    VulkanComponentFactory& vulkanComponentFactory = VulkanComponentFactory::getInstance();


    _renderPass = createRenderPass();

    createGraphicsPipelines(_renderPass);
    createFrameBuffer(_renderPass);

    vulkanComponentFactory.createCommandPool();
    _commandBuffers =  vulkanComponentFactory.allocateCommandBuffer();

    createSyncObjects();
}

VkShaderModule Renderer::createShaderModule(std::vector<char> &shader)
{
    VkShaderModuleCreateInfo shaderModuleCreateInfo{};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.pNext = nullptr;
    shaderModuleCreateInfo.codeSize = shader.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<uint32_t*>(shader.data()); // ~ the mean by aligment requirment ?


    VkShaderModule shaderModule = VK_NULL_HANDLE;
    vkCreateShaderModule(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , &shaderModuleCreateInfo , nullptr , &shaderModule);

    return shaderModule;
}

VkRenderPass Renderer::createRenderPass()
{
    VkAttachmentDescription colorAttachmentDescription{};
    colorAttachmentDescription.format = VulkanComponentFactory::getInstance().getDesiredSurfaceFormat().format;
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &colorAttachmentDescription;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    VkRenderPass renderPass = VK_NULL_HANDLE;
    vkCreateRenderPass(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , &renderPassCreateInfo , nullptr , &renderPass);

    return renderPass;
}

void Renderer::createGraphicsPipelines(const VkRenderPass renderPass)
{
    const VkShaderModule vertexShaderModule = createShaderModule(_vertexShader);

    VkPipelineShaderStageCreateInfo pipelineVertexShaderStageCreateInfo{};
    pipelineVertexShaderStageCreateInfo.pName = "main";
    pipelineVertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineVertexShaderStageCreateInfo.pNext = nullptr;
    pipelineVertexShaderStageCreateInfo.module = vertexShaderModule;
    pipelineVertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    pipelineVertexShaderStageCreateInfo.pSpecializationInfo = nullptr; // ?


    const VkShaderModule fragmentShaderModule = createShaderModule(_fragmentShader);

    VkPipelineShaderStageCreateInfo pipelineFragmentShaderStageCreateInfo{};
    pipelineFragmentShaderStageCreateInfo.pName = "main";
    pipelineFragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineFragmentShaderStageCreateInfo.pNext = nullptr;
    pipelineFragmentShaderStageCreateInfo.module = fragmentShaderModule;
    pipelineFragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    pipelineFragmentShaderStageCreateInfo.pSpecializationInfo = nullptr; // ?

    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[] = {pipelineVertexShaderStageCreateInfo , pipelineFragmentShaderStageCreateInfo};

    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT , VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
    pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStates.size();
    pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates.data();
    pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineDynamicStateCreateInfo.pNext = nullptr;

    VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo{};
    pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pipelineVertexInputStateCreateInfo.pNext = nullptr;
    pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;       //
    pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;         //
    pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions= nullptr;       //    Hared coded in shader.
    pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr;    //

    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.height = static_cast<float>(VulkanComponentFactory::getInstance().getSurfaceCapabilities().currentExtent.height);
    viewport.width = static_cast<float>(VulkanComponentFactory::getInstance().getSurfaceCapabilities().currentExtent.width);
    viewport.maxDepth = 0.0f;
    viewport.minDepth = 1.0f;
    viewport.x = 0.0f;
    viewport.y = 0.0f;

    VkRect2D scissor{};
    scissor.extent = VulkanComponentFactory::getInstance().getSurfaceCapabilities().currentExtent;
    scissor.offset = {0 , 0};

    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
    pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfo.viewportCount = 1;
    pipelineViewportStateCreateInfo.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
    pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
    pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
    pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
    pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
    pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
    pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
    pipelineColorBlendStateCreateInfo.attachmentCount = 1;
    pipelineColorBlendStateCreateInfo.pAttachments = &colorBlendAttachment;
    pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 0;
    pipelineLayoutCreateInfo.pSetLayouts = nullptr;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    vkCreatePipelineLayout(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , &pipelineLayoutCreateInfo , nullptr , &pipelineLayout);

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{};
    graphicsPipelineCreateInfo.layout = pipelineLayout;
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.stageCount = 2;
    graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfos;
    graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
    graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
    graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
    graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
    graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
    graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
    graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
    graphicsPipelineCreateInfo.renderPass = renderPass;
    graphicsPipelineCreateInfo.subpass = 0;

    _graphicsPipeline = VK_NULL_HANDLE;
    vkCreateGraphicsPipelines(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , VK_NULL_HANDLE , 1 , &graphicsPipelineCreateInfo , nullptr , &_graphicsPipeline);
}

void Renderer::createFrameBuffer(VkRenderPass renderPass)
{
    const std::vector<VkImageView> imageViews = PresentationEngine::getInstance().getImageViews();

    for(int i = 0 ; i < imageViews.size() ; ++i)
    {
        VkImageView attachments =  imageViews[i];

        VkFramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = &attachments;
        framebufferCreateInfo.layers = 1;
        framebufferCreateInfo.width = VulkanComponentFactory::getInstance().getSurfaceCapabilities().currentExtent.width;
        framebufferCreateInfo.height = VulkanComponentFactory::getInstance().getSurfaceCapabilities().currentExtent.height;


        VkFramebuffer framebuffer = VK_NULL_HANDLE;
        vkCreateFramebuffer(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice() , &framebufferCreateInfo , nullptr , &framebuffer);

        _framebuffers.push_back(framebuffer);
    }
}

void Renderer::createSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice(), &fenceInfo, nullptr, &_inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create synchronization objects for a frame!");
    }
}

void Renderer::drawFrame()
{
    vkWaitForFences(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice(), 1, &_inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice(), 1, &_inFlightFence);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(VulkanComponentFactory::getInstance().getCreatedVulkanLogicalDevice(), VulkanComponentFactory::getInstance().getSwapchain(), UINT64_MAX, _imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(_commandBuffers.at(0), 0);
    recordCommandBuufer(_renderPass, imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {_imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = _commandBuffers.data();

    VkSemaphore signalSemaphores[] = {_renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    VkQueue graphicQueue = VulkanComponentFactory::getInstance().getDeviceQueue(VulkanComponentFactory::getInstance().queueFamilyIndices.graphicQueueFamilyIndex);

    vkQueueSubmit(graphicQueue , 1, &submitInfo, _inFlightFence);

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {VulkanComponentFactory::getInstance().getSwapchain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    VkQueue presentQueue = VulkanComponentFactory::getInstance().getDeviceQueue(VulkanComponentFactory::getInstance().queueFamilyIndices.presentationQueueFamilyIndex);

    vkQueuePresentKHR(presentQueue, &presentInfo);
}

} // RenderingEngine

