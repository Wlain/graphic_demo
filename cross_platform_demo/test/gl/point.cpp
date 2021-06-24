//
// Created by william on 2021/6/23.
//

#include "point.h"
namespace graphicEngine::gl
{
Point::~Point() = default;

void Point::initialize()
{
    initWithProperty(std::make_tuple("point", "point.gl.vert", "point.gl.frag"));
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void Point::update(float elapseTime)
{
    CommonInterface::update(elapseTime);
}

void Point::resize(int width, int height)
{
    CommonInterface::resize(width, height);
}

void Point::display()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_program->getProgram());
    glBindVertexArray(m_vao);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

} // namespace graphicEngine::gl