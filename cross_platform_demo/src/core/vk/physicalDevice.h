//
// Created by william on 2021/8/8.
//

#ifndef CROSS_PLATFORM_DEMO_PHYSICALDEVICE_H
#define CROSS_PLATFORM_DEMO_PHYSICALDEVICE_H
#include "commonHandle_vk.h"

namespace graphicEngine::vk
{
class Instance;
/**
 * @brief VkPhysicalDevice类的封装
 *
 * 该类负责处理设备创建的gpu特征、属性和队列系列。
 */
class PhysicalDevice
{
public:
    PhysicalDevice(Instance& instance, VkPhysicalDevice& physicalDevice);
private:
};
} // namespace graphicEngine::vk

#endif //CROSS_PLATFORM_DEMO_PHYSICALDEVICE_H
