//
// Created by william on 2021/6/24.
//

#ifndef CROSS_PLATFORM_DEMO_SHOWGLINFO_H
#define CROSS_PLATFORM_DEMO_SHOWGLINFO_H
#include "commonInterface.h"

namespace graphicEngine::gl
{
class ShowGLInfo : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~ShowGLInfo() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_SHOWGLINFO_H
