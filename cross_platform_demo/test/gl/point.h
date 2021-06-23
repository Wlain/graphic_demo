//
// Created by william on 2021/6/23.
//
//    demo:
//    Point point;
//    point.initialize();
//    point.draw();

#ifndef CROSS_PLATFORM_DEMO_POINT_H
#define CROSS_PLATFORM_DEMO_POINT_H
#include "commonInterface.h"

namespace graphicEngine::gl
{
class Point : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~Point() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void draw() override;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_POINT_H
