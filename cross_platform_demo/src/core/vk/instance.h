//
// Created by william on 2021/6/19.
//

#ifndef CROSS_PLATFORM_DEMO_INSTANCE_H
#define CROSS_PLATFORM_DEMO_INSTANCE_H
#include "commonDefine_vk.h"
#include "layerAndExtension.h"
#include "resourceCache.h"

class PhysicalDevice;
/**
 * @brief 返回Khronos/LunarG支持的验证层的列表,尝试按照优先顺序启用它们，从较晚的Vulkan SDK版本开始。
 * @param supportedInstanceLayers 要检查的验证层的列表
 * @return 验证层列表
 */
std::vector<const char*> getOptimalValidationLayers(const std::vector<VkLayerProperties>& supportedInstanceLayers);

class Instance
{
public:
    Instance();
    ~Instance();
    /// 不可复制

    VkResult createInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* applicationName);
    void destroyInstance();
    Instance(const Instance&) = delete;
    /// 不可移动
    Instance(Instance&&) = delete;
    /// 不可赋值
    Instance& operator=(const Instance&) = delete;
    Instance& operator=(Instance&&) = delete;
    inline operator VkInstance() { return m_handle; }
    inline operator const VkInstance() const { return m_handle; }
    inline VkInstance handle() { return m_handle; }
    inline VkInstance handle() const { return m_handle; }
    inline const VkAllocationCallbacks* allocator() { return m_allocator; }

    PhysicalDevice& getSuitableGpu(VkSurfaceKHR);
    PhysicalDevice& getFirstGpu();
    bool isEnabled(const char* extension) const;

private:

public:
    /// vulkan实例对象的变量
    VkInstance m_handle = VK_NULL_HANDLE;
    /// 物理设备
    std::vector<std::unique_ptr<PhysicalDevice>> m_gpus;
    /// 层和扩展
    LayerAndExtension m_layerExtension;
    VkAllocationCallbacks* m_allocator = VK_NULL_HANDLE;
};

#endif //CROSS_PLATFORM_DEMO_INSTANCE_H
