//
// Created by william on 2021/6/25.
//

#include "instancing.h"

namespace graphicEngine::gl
{
Instancing::~Instancing() = default;

void Instancing::initialize()
{
    m_property = {"instancing", "instancing.vert", "instancing.frag"};
    Cube::initialize();
    m_camera.z = 420.0f;
}

void Instancing::update(float elapseTime)
{
    Cube::update(elapseTime);
}

void Instancing::resize(int width, int height)
{
    Cube::resize(width, height);
}

void Instancing::display()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    m_vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_camera.x, -m_camera.y, -m_camera.z));
    m_program->setMatrix4("v_matrix", m_vMat);
    m_program->setMatrix4("proj_matrix", m_pMat);

    m_timeFactor = ((float)m_currentTime);
    m_program->setFloat("tf", m_timeFactor);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(m_vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000); // 0, 36, 24  (or 100000)
}
} // namespace graphicEngine::gl