//
// Created by william on 2021/8/15.
//

#ifndef CROSS_PLATFORM_DEMO_PIPELINE_H
#define CROSS_PLATFORM_DEMO_PIPELINE_H
#include "commonDefine_vk.h"
#include "pipelineState.h"
class Device;

class Pipeline
{
public:
    Pipeline(Device &device);
    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&& other);
    virtual ~Pipeline();
    // 禁止赋值
    Pipeline& operator=(const Pipeline&) = delete;
    // 禁止赋值
    Pipeline& operator=(Pipeline&&) = delete;
    inline VkPipeline handle() const { return m_handle; }
    inline const PipelineState& state() const { return m_state; }

protected:
    Device& m_device;
    VkPipeline m_handle = VK_NULL_HANDLE;
    PipelineState m_state;
};

class GraphicPipeline : public Pipeline
{
public:
    GraphicPipeline();
    ~GraphicPipeline() = default;
};

class ComputePipeline : public Pipeline
{
public:
    ComputePipeline();
    ~ComputePipeline() = default;
};

#endif //CROSS_PLATFORM_DEMO_PIPELINE_H
