//
// Created by william on 2021/6/25.
//

#ifndef CROSS_PLATFORM_DEMO_CUBE_H
#define CROSS_PLATFORM_DEMO_CUBE_H
#include "commonInterface.h"
namespace graphicEngine::gl
{
class Cube : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~Cube() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;

protected:
    float m_vertexPositions[108] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
    };
    glm::vec3 m_camera{0.0f, 0.0f, 30.0f};
    glm::vec3 m_target{0.0f, -2.0f, 0.0f};
    glm::mat4 m_pMat, m_vMat, m_mMat, m_mvMat, m_tMat, m_rMat;
    std::tuple<const char*, std::string, std::string> m_property{"cube", "cube.vert", "cube.frag"};
    int m_displayLoopi = 0;
    float m_currentTime;
    float m_tf = 0.0f;
};
} // namespace graphicEngine::gl

#endif //CROSS_PLATFORM_DEMO_CUBE_H
