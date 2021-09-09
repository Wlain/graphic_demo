//
// Created by ytech on 2021/9/8.
//

#ifndef CROSS_PLATFORM_DEMO_WRAPPER_H
#define CROSS_PLATFORM_DEMO_WRAPPER_H

#include "commonDefine_vk.h"

class CommandBufferMgr
{
public:
    static void allocCommandBuffer(const VkDevice* device, const VkCommandPool cmdPool, VkCommandBuffer* cmdBuffer, const VkCommandBufferAllocateInfo* inCmdBufferInfo = nullptr);
    static void beginCommonBuffer(VkCommandBuffer cmdBuffer, VkCommandBufferBeginInfo* inCommandBufferInfo = nullptr);
    static void endCommonBuffer(VkCommandBuffer);
    static void submitCommandBuffer(const VkQueue& queue, const VkCommandBuffer* cmdBufferLists, const VkSubmitInfo* inSubmitInfo = nullptr, const VkFence& fence = nullptr);
};

#endif //CROSS_PLATFORM_DEMO_WRAPPER_H

