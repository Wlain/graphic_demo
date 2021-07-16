//
// Created by william on 2021/6/25.
//

#ifndef CROSS_PLATFORM_DEMO_SPHERE_H
#define CROSS_PLATFORM_DEMO_SPHERE_H
#include "commonInterface.h"

namespace graphicEngine::gl
{
class sphere : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~sphere() override;
public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;
};
}

#endif //CROSS_PLATFORM_DEMO_SPHERE_H
