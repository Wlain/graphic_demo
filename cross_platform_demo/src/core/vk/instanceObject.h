//
// Created by william on 2021/6/19.
//

#ifndef CROSS_PLATFORM_DEMO_INSTANCEOBJECT_H
#define CROSS_PLATFORM_DEMO_INSTANCEOBJECT_H
#include "base.h"
#include "vulkan/vulkan_core.h"
#define ENABLE_VALIDATION_LAYERS 1

class InstanceObject
{
public:
    InstanceObject();
    ~InstanceObject();
    /// 不可复制
    InstanceObject(const InstanceObject&) = delete;
    /// 不可移动
    InstanceObject(InstanceObject&&) = delete;
    /// 不可赋值
    InstanceObject& operator=(const InstanceObject&) = delete;
    InstanceObject& operator=(InstanceObject&&) = delete;
    inline operator VkInstance() { return m_handler; }
    inline operator const VkInstance() const { return m_handler; }
    inline VkInstance handler() { return m_handler; }
    inline VkInstance handler() const { return m_handler; }
    inline const VkAllocationCallbacks* allocator() { return m_allocator; }
    inline std::vector<const char*>& enabledExtensions() { return m_enabledExtensions; };
    inline std::vector<const char*>& validationLayers() { return m_validationLayers; };

private:
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

public:
    /// glfw窗口
    GLFWwindow* m_window = VK_NULL_HANDLE;
    /// vulkan实例对象的变量
    VkInstance m_handler = VK_NULL_HANDLE;
    /// 可用扩展
    std::vector<const char*> m_enabledExtensions;
    /// 验证层
    std::vector<const char*> m_validationLayers;
    /// debug调试信息
    VkDebugUtilsMessengerEXT m_debugUtilsMessenger = VK_NULL_HANDLE;
    /// debug回调report
    VkDebugReportCallbackEXT m_debugReportCallback = VK_NULL_HANDLE;
    VkAllocationCallbacks* m_allocator = VK_NULL_HANDLE;
};

#endif //CROSS_PLATFORM_DEMO_INSTANCEOBJECT_H
