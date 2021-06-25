//
// Created by william on 2021/6/25.
//

#include "cube.h"
namespace graphicEngine::gl
{
Cube::~Cube() = default;

void Cube::initialize()
{
    initWithProperty(std::make_tuple("cube", "cube.vert", "cube.frag"));
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexPositions), m_vertexPositions, GL_STATIC_DRAW);
}

void Cube::update(float elapseTime)
{
    CommonInterface::update(elapseTime);
}

void Cube::resize(int width, int height)
{
    float aspect = (float)width / (float)height;
    m_pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void Cube::display()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    m_program->use();
    m_vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_camera.x, -m_camera.y, -m_camera.z));
    m_mMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_target.x, m_target.y, m_target.z));
    m_mvMat = m_vMat * m_mMat;
    m_program->setMatrix4("mv_matrix", m_mvMat);
    m_program->setMatrix4("proj_matrix", m_pMat);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
}