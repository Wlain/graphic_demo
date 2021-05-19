//
// Created by william on 2021/4/5.
//

#include "sharing.h"

#include <random>

static std::random_device r;
// 选择 1 与 6 间的随机数
static std::default_random_engine e1(r());
static std::uniform_int_distribution<int> uniformDist(0, 100);

auto randomTest()
{
    return uniformDist(e1);
}

namespace graphicEngine::gl
{

Sharing::~Sharing()
{
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Sharing::initialize()
{
    initWithProperty(std::make_tuple("First", "sharing.gl.vert", "sharing.gl.frag"));
    m_window1 = glfwCreateWindow(400, 400, "Second", nullptr, m_window);
    if (!m_window1)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // Create the OpenGL objects inside the first context, created above
    // All objects will be shared with the second context, created below
    {
        char pixels[16 * 16];
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        srand((unsigned int)glfwGetTimerValue());
        for (int i = 0; i < 16; ++i)
        {
            for (int j = 0; j < 16; ++j)
            {
                pixels[i * 16 + j] = randomTest() % 256;
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 16, 16, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
    }
    glUseProgram(m_program->getProgram());
    glUniform1i(glGetUniformLocation(m_program->getProgram(), "texture"), 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);
//    glGenVertexArrays(1, &m_vao);
//    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), (void*)0);

    // Place the second window to the right of the first
    {
        int xPos, yPos, left, right, width;
        glfwGetWindowSize(m_window, &width, nullptr);
        glfwGetWindowFrameSize(m_window, &left, nullptr, &right, nullptr);
        glfwGetWindowPos(m_window, &xPos, &yPos);

        glfwSetWindowPos(m_window1, xPos + width + left + right, yPos);
    }
    glfwSetKeyCallback(m_window1, keyCallback);
    glfwMakeContextCurrent(m_window1);
    // While objects are shared, the global context state is not and will
    // need to be set up for each context
    glUseProgram(m_program->getProgram());
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), (void*) 0);
}

void Sharing::update(float elapseTime)
{
}

void Sharing::resize(int width, int height)
{
}

void Sharing::draw()
{
    while (!glfwWindowShouldClose(m_window) && !glfwWindowShouldClose(m_window1))
    {
        for (int i = 0; i < 2; ++i)
        {
            int width, height;
            mat4x4 mvp;
            glfwGetFramebufferSize(i == 0 ? m_window : m_window1, &width, &height);
            glViewport(0, 0, width, height);
            mat4x4_ortho(mvp, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f);
            glUniformMatrix4fv(glGetUniformLocation(m_program->getProgram(), "MVP"), 1, GL_FALSE, (const GLfloat*) mvp);
            glUniform3fv(glGetUniformLocation(m_program->getProgram(), "color"), 1, m_colors[i]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glfwSwapBuffers(i == 0 ? m_window : m_window1);
        }
        glfwWaitEvents();
    }
}

} // namespace graphicEngine::gl