//
// Created by william on 2021/4/5.
//

#ifndef CROSS_PLATFORM_DEMO_TRIANGLE_H
#define CROSS_PLATFORM_DEMO_TRIANGLE_H

#include "commonInterface.h"

namespace graphicEngine::gl
{
class Triangle : public CommonInterface
{
public:
    typedef struct Vertex
    {
        vec2 pos;
        vec3 col;
    } Vertex;

public:
    using CommonInterface::CommonInterface;
    ~Triangle() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;

private:
    Vertex m_vertices[3] = {
        { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
        { { 0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
        { { 0.0f, 0.6f }, { 0.f, 0.f, 1.f } }
    };
    mat4x4 m_mvpMatrix;
    float m_ratio = 0.0f;
};

} // namespace graphicEngine::gl
#endif //CROSS_PLATFORM_DEMO_TRIANGLE_H
