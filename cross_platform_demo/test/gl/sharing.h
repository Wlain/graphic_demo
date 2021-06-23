//
// Created by william on 2021/4/5.
//

#ifndef CROSS_PLATFORM_DEMO_SHARING_H
#define CROSS_PLATFORM_DEMO_SHARING_H

#include "commonInterface.h"

namespace graphicEngine::gl
{
class Sharing : public CommonInterface
{
public:
    using CommonInterface = CommonInterface;
    ~Sharing() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void draw() override;

private:
    const vec2 m_vertices[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };
    const vec3 m_colors[2] = {
        { 0.8f, 0.4f, 1.f },
        { 0.3f, 0.4f, 1.f }
    };
    GLFWwindow* m_window1;
    GLuint m_texture = 0;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_SHARING_H
