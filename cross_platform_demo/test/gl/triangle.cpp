//
// Created by william on 2021/4/5.
//

#include "triangle.h"

using namespace graphicEngine;

namespace graphicEngine::gl
{
Triangle::~Triangle()
{
}

void Triangle::initialize()
{
    initWithProperty(std::make_tuple("OpenGL Triangle", "triangle.gl.vert", "triangle.gl.frag"));
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
}

void Triangle::update(float elapseTime)
{
    mat4x4 m, p;
    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, elapseTime);
    mat4x4_ortho(p, -m_ratio, m_ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    mat4x4_mul(m_mvpMatrix, p, m);
}

void Triangle::resize(int width, int height)
{
    m_ratio = (float)width / (float)height;
    glViewport(0.0f, 0.0f, width, height);
}

void Triangle::draw()
{
    while (!glfwWindowShouldClose(m_window))
    {
        CommonInterface::draw();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(m_program->getProgram());
        glUniformMatrix4fv(glGetUniformLocation(m_program->getProgram(), "MVP"), 1, GL_FALSE, (const GLfloat*)&m_mvpMatrix);
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

} // namespace graphicEngine::gl