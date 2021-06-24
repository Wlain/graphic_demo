//
// Created by william on 2021/6/24.
//

#ifndef CROSS_PLATFORM_DEMO_ANIMATION_H
#define CROSS_PLATFORM_DEMO_ANIMATION_H
#include "commonInterface.h"
#include "utils.h"

namespace graphicEngine::gl
{
class Animation : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~Animation() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;

private:
    float m_offsetX = 0.0f;
    float m_inc = 0.01f;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_ANIMATION_H
