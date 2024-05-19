#include <VulkanFunctionsPointersInitializer.h>

using namespace RenderingEngine;

int main()
{
    VulkanFunctionsPointersInitializer& vulkanFunctionsPointersInitializer = VulkanFunctionsPointersInitializer::getInstance();

    vulkanFunctionsPointersInitializer.initializeVulkanFunctionsPointer();
}
