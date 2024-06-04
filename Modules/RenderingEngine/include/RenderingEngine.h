#ifndef    RENDERING_ENGINE_H
#define    RENDERING_ENGINE_H

#include <vector>
#include <vulkan/vulkan.h>

namespace RenderingEngine
{

class Renderer
{

private:
    Renderer();
    ~Renderer();

public:
    static Renderer& getInstance();
    void init();
    void drawFrame();

private:
    VkShaderModule createShaderModule(std::vector<char>& shader);
    VkRenderPass createRenderPass();
    void recordCommandBuufer(VkRenderPass renderPass, uint32_t imageIndex);
    void createGraphicsPipelines(const VkRenderPass renderPass);
    void createFrameBuffer(VkRenderPass renderPass);
    void createSyncObjects();

    std::vector<char> _fragmentShader;
    std::vector<char> _vertexShader;
    std::vector<VkFramebuffer> _framebuffers;
    VkPipeline  _graphicsPipeline = VK_NULL_HANDLE;
    VkSemaphore _imageAvailableSemaphore;
    VkSemaphore _renderFinishedSemaphore;
    VkFence     _inFlightFence;
    std::vector<VkCommandBuffer> _commandBuffers;
    VkRenderPass _renderPass = VK_NULL_HANDLE;
};

} // RenderingEngine

#endif  // RENDERING_ENGINE_H
