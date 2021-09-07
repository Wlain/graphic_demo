//
// Created by william on 2021/6/20.
//

#ifndef CROSS_PLATFORM_DEMO_VULKANAPPLICATION_H
#define CROSS_PLATFORM_DEMO_VULKANAPPLICATION_H
#include "device2.h"
#include "instance.h"
#include "layerAndExtension.h"

class VulkanApplication
{
private:
    VulkanApplication();

public:
    ~VulkanApplication();

public:
    static VulkanApplication* getInstance();

    void initialize(); // Initialize and allocate resources
    void prepare();    // Prepare resource
    void update();     // Update data
    bool render();     // Render primitives
    void destroy();    // Release resources

private:
    // Create the Vulkan instance object
    VkResult createVulkanInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* applicationName);
    VkResult handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char*>& layers, std::vector<const char*>& extensions);
    void enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuLists);

private:
    static std::unique_ptr<VulkanApplication> s_instance;
    static std::once_flag s_onlyOnce;

public:
    Instance m_instanceObj; // Vulkan Instance object
    Device* m_deviceObj = nullptr;
private:
    bool m_isDebug = false;
};

#endif //CROSS_PLATFORM_DEMO_VULKANAPPLICATION_H
