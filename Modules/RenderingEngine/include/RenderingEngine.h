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
    void draw();

private:
    VkShaderModule createShaderModule(std::vector<char>& shader);
    VkRenderPass createRenderPass();
    void createGraphicsPipelines(const VkRenderPass renderPass);
    void createFrameBuffer(VkRenderPass renderPass);

    std::vector<char> _fragmentShader;
    std::vector<char> _vertexShader;
    std::vector<VkFramebuffer> _framebuffers;
    VkPipeline _graphicsPipeline = VK_NULL_HANDLE;
};

} // RenderingEngine

#endif  // RENDERING_ENGINE_H
