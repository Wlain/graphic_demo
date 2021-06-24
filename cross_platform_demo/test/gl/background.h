//
// Created by william on 2021/6/23.
//

#ifndef CROSS_PLATFORM_DEMO_BACKGROUND_H
#define CROSS_PLATFORM_DEMO_BACKGROUND_H
#include "commonInterface.h"
namespace graphicEngine::gl
{
class Background : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~Background() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_BACKGROUND_H
