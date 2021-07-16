//
// Created by william on 2021/6/25.
//

#include "cube.h"
namespace graphicEngine::gl
{
Cube::~Cube() = default;

void Cube::initialize()
{
    initWithProperty(m_property);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexPositions), m_vertexPositions, GL_STATIC_DRAW);
}

void Cube::update(float elapseTime)
{
    m_currentTime = elapseTime;
}

void Cube::resize(int width, int height)
{
    float aspect = (float)width / (float)height;
    m_pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void Cube::display()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    m_vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_camera.x, -m_camera.y, -m_camera.z));
    m_program->setMatrix4("proj_matrix", m_pMat);
    for (m_displayLoopi = 0; m_displayLoopi < 24; m_displayLoopi++)
    {
        m_tf = m_currentTime + m_displayLoopi;
        m_tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(.35f * m_tf) * 8.0f, cos(.52f * m_tf) * 8.0f, sin(.70f * m_tf) * 8.0f));
        m_rMat = glm::rotate(glm::mat4(1.0f), 1.75f * m_tf, glm::vec3(0.0f, 1.0f, 0.0f));
        m_rMat = glm::rotate(m_rMat, 1.75f * m_tf, glm::vec3(1.0f, 0.0f, 0.0f));
        m_rMat = glm::rotate(m_rMat, 1.75f * m_tf, glm::vec3(0.0f, 0.0f, 1.0f));
        m_mMat = m_tMat * m_rMat;
        m_mvMat = m_vMat * m_mMat;
        m_program->setMatrix4("mv_matrix", m_mvMat);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void *)0);
        glEnableVertexAttribArray(0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

} // namespace graphicEngine::gl