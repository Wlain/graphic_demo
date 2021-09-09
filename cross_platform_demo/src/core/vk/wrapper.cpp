//
// Created by ytech on 2021/9/8.
//

#include "wrapper.h"

void CommandBufferMgr::allocCommandBuffer(const VkDevice* device, const VkCommandPool cmdPool, VkCommandBuffer* cmdBuffer, const VkCommandBufferAllocateInfo* inCmdBufferInfo)
{
    if (inCmdBufferInfo)
    {
        VK_CHECK(vkAllocateCommandBuffers(*device, inCmdBufferInfo, cmdBuffer));
        return;
    }
    VkCommandBufferAllocateInfo cmdInfo{};
    cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdInfo.pNext = nullptr;
    cmdInfo.commandPool = cmdPool;
    cmdInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdInfo.commandBufferCount = (uint32_t)sizeof(cmdBuffer) / sizeof(VkCommandBuffer);
    VK_CHECK(vkAllocateCommandBuffers(*device, &cmdInfo, cmdBuffer));
}

void CommandBufferMgr::beginCommonBuffer(VkCommandBuffer cmdBuffer, VkCommandBufferBeginInfo* inCommandBufferInfo)
{
    if (inCommandBufferInfo)
    {
        VK_CHECK(vkBeginCommandBuffer(cmdBuffer, inCommandBufferInfo));
    }
    VkCommandBufferInheritanceInfo cmdBufInheritanceInfo{};
    cmdBufInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufInheritanceInfo.pNext = nullptr;
    cmdBufInheritanceInfo.renderPass = VK_NULL_HANDLE;
    cmdBufInheritanceInfo.subpass = 0;
    cmdBufInheritanceInfo.framebuffer = VK_NULL_HANDLE;
    cmdBufInheritanceInfo.occlusionQueryEnable = VK_FALSE;
    cmdBufInheritanceInfo.queryFlags = 0;
    cmdBufInheritanceInfo.pipelineStatistics = 0;

    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = VK_NULL_HANDLE;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = &cmdBufInheritanceInfo;

    VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &commandBufferBeginInfo));
}

void CommandBufferMgr::endCommonBuffer(VkCommandBuffer cmdBuffer)
{
    VK_CHECK(vkEndCommandBuffer(cmdBuffer));
}

void CommandBufferMgr::submitCommandBuffer(VkQueue const& queue, const VkCommandBuffer* cmdBufferLists, const VkSubmitInfo* inSubmitInfo, VkFence const& fence)
{
    if (inSubmitInfo)
    {
        VK_CHECK(vkQueueSubmit(queue, 1, inSubmitInfo, fence));
        VK_CHECK(vkQueueWaitIdle(queue));
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.commandBufferCount = (uint32_t)sizeof(cmdBufferLists)/sizeof(VkCommandBuffer);
    submitInfo.pCommandBuffers = cmdBufferLists;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;
    VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, fence));
    VK_CHECK(vkQueueWaitIdle(queue));
}