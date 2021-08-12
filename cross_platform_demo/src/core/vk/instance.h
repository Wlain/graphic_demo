//
// Created by william on 2021/6/19.
//

#ifndef CROSS_PLATFORM_DEMO_INSTANCE_H
#define CROSS_PLATFORM_DEMO_INSTANCE_H
#include "commonHandle_vk.h"

namespace graphicEngine::vk
{
class PhysicalDevice;

/**
 * @brief 返回Khronos/LunarG支持的验证层的列表
 *       尝试按照优先顺序启用它们，从较晚的Vulkan SDK版本开始。
 * @param getOptimalValidationLayers 要检查的验证层的列表
 */
std::vector<const char*> getOptimalValidationLayers(const std::vector<VkLayerProperties>& supportedInstanceLayers);
/**
 * @brief VkInstance的一个封装类。
 *
 * 这个类负责初始化volk，枚举所有可用的扩展和验证层，如果存在的话，将它们启用。
 * 如果存在的话，将其启用，设置调试信息，并查询机器上现有的所有物理设备。
 */
class Instance
{
public:
    /**
     * @brief 初始化与Vulkan的连接
     * @param applicationName  应用程序的名称
     * @param requiredExtensions 请求启用的扩展功能
     * @param requiredValidationLayers 要启用的验证层
     * @param headless 应用程序是否要求进行headless设置
     * @param apiVersion 实例将使用的Vulkan API版本
     * @throws 如果没有找到所需的扩展和验证层，则抛出运行时间错误
     */
    Instance(const std::string& applicationName, const std::unordered_map<const char*, bool>& requiredExtensions = {}, const std::vector<const char*>& requiredValidationLayers = {}, bool headless = false, uint32_t apiVersion = VK_API_VERSION_1_0);
    /**
     * @brief 查询已经创建的VkInstance的GPU
     * @param instance A valid VkInstance
     */
    Instance(VkInstance instance);

    Instance(const Instance&) = delete;

    Instance(Instance&&) = delete;

    ~Instance();

    Instance& operator=(const Instance&) = delete;

    Instance& operator=(Instance&&) = delete;

    /**
     * @brief 查询机器上物理设备的实例
     */
    void queryGPUs();

    /**
     * @brief 试图找到第一个可以渲染给定表面的游离GPU
     * @param surface ：要测试的表面
     * @return 一个有效的物理设备
     */
    PhysicalDevice& getSuitableGPU(VkSurfaceKHR surface);

    /**
     * @brief 试图找到第一个可用的独立GPU
     * @returns 一个有效的物理设备
     */
    PhysicalDevice& getFirstGPU();

    /**
     * @brief 检查给定的扩展是否在VkInstance中被启用
     * @param extension 指定扩展
     */
    bool isEnabled(const char* extension) const;

    VkInstance getHandle();

    const std::vector<const char*>& getExtensions();

private:
    /**
     * @brief Vulkan实例
     */
    VkInstance m_handle{ VK_NULL_HANDLE };
    /**
     * @brief 可用扩展
     */
    std::vector<const char*> m_enabledExtensions;
    /**
	 * @brief Debug utils messenger callback for VK_EXT_Debug_Utils
	 */
    VkDebugUtilsMessengerEXT m_debugUtilsMessenger{ VK_NULL_HANDLE };
    /**
	 * @brief The debug report callback
	 */
    VkDebugReportCallbackEXT m_debugReportCallback{ VK_NULL_HANDLE };
};
} // namespace graphicEngine::vk

#endif //CROSS_PLATFORM_DEMO_INSTANCE_H
