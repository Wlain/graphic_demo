//
// Created by william on 2021/8/15.
//

#ifndef CROSS_PLATFORM_DEMO_COMMONDEFINE_VK_H
#define CROSS_PLATFORM_DEMO_COMMONDEFINE_VK_H

#include "base.h"
#include <vulkan/vulkan.h>
#define ENABLE_VALIDATION_LAYERS 1

/// VK_CHECK macros
#define VK_CHECK(expression)                                                              \
    do                                                                                    \
    {                                                                                     \
        VkResult ASSERT_ONLY error = expression;                                          \
        if (error)                                                                        \
        {                                                                                 \
            LOG_ERROR("VkResult: (%d) :{}: Vulkan Assertion Failed", error, #expression); \
        }                                                                                 \
    } while (0)

#endif //CROSS_PLATFORM_DEMO_COMMONDEFINE_VK_H
