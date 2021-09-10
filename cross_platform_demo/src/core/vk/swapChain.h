//
// Created by ytech on 2021/9/9.
//

#ifndef CROSS_PLATFORM_DEMO_SWAPCHAIN_H
#define CROSS_PLATFORM_DEMO_SWAPCHAIN_H
#include "commonDefine_vk.h"

class Instance;
class Device;
class Renderer;
class VulkanApplication;

struct SwapChainBuffer
{
    VkImage image;
    VkImageView view;
};

struct SwapChainPrivateVariable
{
    /// 存储表面
    VkSurfaceCapabilitiesKHR  surfaceCapabilities;
    uint32_t presentModeCount;
    std::vector<VkPresentModeKHR> presentModes;
    VkExtent2D swapChainExtent; // 宽高
    uint32_t desiredNumberOfSwapChainImages;
    VkSurfaceTransformFlagBitsKHR preTransform;
    VkPresentModeKHR swapChainPresentMode;
    std::vector<VkImage> swapChainImages;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
};

struct SwapChainPublicVariable
{
    VkSurfaceKHR surface;
    uint32_t swapChainImagesCount;
    VkSwapchainKHR swapChain; // 交换链句柄
    std::vector<SwapChainBuffer> colorBuffers; // 交换链图片列表
    VkSemaphore presentCompleteSemaphore; // 同步信号量
    uint32_t currentColorBuffer; // 当前绘制表面的索引
    VkFormat format; // 图片格式
};

class SwapChain
{
public:
    SwapChain(Renderer* renderer);
    ~SwapChain();
    void initializeSwapChain(const VkCommandBuffer& cmd);
    void createSwapChain(const VkCommandBuffer& cmd);
};

#endif //CROSS_PLATFORM_DEMO_SWAPCHAIN_H
