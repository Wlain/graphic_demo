//
// Created by william on 2021/4/5.
//

#ifndef CROSS_PLATFORM_DEMO_TRIANGLE_H
#define CROSS_PLATFORM_DEMO_TRIANGLE_H

#include "commonInterface.h"

namespace graphicEngine::vk
{
class Triangle : CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~Triangle() override;

public:
    void initialize() override;
    void draw() override;
};

} // namespace graphicEngine::vk

#endif //CROSS_PLATFORM_DEMO_TRIANGLE_H
