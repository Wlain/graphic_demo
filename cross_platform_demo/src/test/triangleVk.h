//
// Created by william on 2021/5/18.
//

#ifndef CROSS_PLATFORM_DEMO_TRIANGLE_VK_H
#define CROSS_PLATFORM_DEMO_TRIANGLE_VK_H
#include "base.h"
#include "vulkan/vulkan_core.h"
#define ENABLE_VALIDATION_LAYERS 1

class TriangleVk
{
public:
    void run();
    void createInstance();
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

private:
    bool checkValidationLayerSupport();
    static std::vector<const char*> getRequiredExtensions();
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

private:
    GLFWwindow* m_window = VK_NULL_HANDLE;
    VkInstance m_instance = VK_NULL_HANDLE;
    VkAllocationCallbacks* m_allocator = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    std::vector<const char*> m_validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    const int WIDTH = 800;
    const int HEIGHT = 600;
};

#endif //CROSS_PLATFORM_DEMO_TRIANGLE_VK_H
