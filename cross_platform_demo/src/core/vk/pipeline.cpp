//
// Created by william on 2021/8/15.
//

#include "pipeline.h"

#include "device.h"

Pipeline::Pipeline(Device& device) :
    m_device{ device }
{
}

Pipeline::Pipeline(Pipeline&& other) :
    m_device{ other.m_device },
    m_handle{ other.m_handle },
    m_state{ other.m_state }
{
    other.m_handle = VK_NULL_HANDLE;
}

Pipeline::~Pipeline()
{
    if (m_handle != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(m_device, m_handle, nullptr);
    }
}
