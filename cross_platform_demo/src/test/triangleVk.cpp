//
// Created by william on 2021/5/18.
//

#include "triangleVk.h"

#include "logger.h"

namespace graphicEngine::vk
{
TriangleVk::TriangleVk() = default;

TriangleVk::~TriangleVk(){
    teardown(m_context);
}

void TriangleVk::teardown(Context& context)
{

}
} // namespace graphicEngine::vk
