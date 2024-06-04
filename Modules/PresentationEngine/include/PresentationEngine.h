#ifndef PRESENTATION_ENGINE_H
#define PRESENTATION_ENGINE_H

#include <glfw3.h>
#include <vector>
#include <vulkan/vulkan.h>

namespace RenderingEngine
{

class PresentationEngine
{
public:
    static PresentationEngine& getInstance();
    void showWindow();
    void init();

    std::vector<VkImageView> getImageViews() const;

private:
    PresentationEngine();
    ~PresentationEngine();

    void getSwapchainImages(std::vector<VkImage>* swapchainImages);
    void createImageViews(std::vector<VkImageView> *imageViews);

    GLFWwindow*          _window = nullptr;
    VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _imageViews;
};

} // RenderingEngine

#endif // PRESENTATION_ENGINE_H
