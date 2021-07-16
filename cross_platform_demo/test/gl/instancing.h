//
// Created by william on 2021/6/25.
//

#ifndef CROSS_PLATFORM_DEMO_INSTANCING_H
#define CROSS_PLATFORM_DEMO_INSTANCING_H
#include "cube.h"
namespace graphicEngine::gl
{
class Instancing : public Cube
{
public:
    using Cube::Cube;
    ~Instancing() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;

public:
    float m_aspect = 0.0f, m_timeFactor = 0.0f;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_INSTANCING_H
