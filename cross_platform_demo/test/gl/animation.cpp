//
// Created by william on 2021/6/24.
//

#include "animation.h"
namespace graphicEngine::gl
{
Animation::~Animation() = default;

void Animation::initialize()
{
    initWithProperty(std::make_tuple("background", "animation.vert", "animation.frag"));
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void Animation::update(float elapseTime)
{
    CommonInterface::update(elapseTime);
}

void Animation::resize(int width, int height)
{
    CommonInterface::resize(width, height);
}

void Animation::display()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_offsetX += m_inc;
    if (m_offsetX > 1.0f) m_inc = -0.01f;
    if (m_offsetX < -1.0f) m_inc = 0.01f;
    m_program->use();
    m_program->setFloat("offset", m_offsetX);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
} // namespace graphicEngine::gl