//
// Created by william on 2021/6/23.
//

#include "background.h"

namespace graphicEngine::gl
{
Background::~Background()
{

}

void Background::initialize()
{
    initWithProperty(std::make_tuple("background", "", ""));
}

void Background::update(float elapseTime)
{
    CommonInterface::update(elapseTime);
}

void Background::resize(int width, int height)
{
    CommonInterface::resize(width, height);
}

void Background::draw()
{
    CommonInterface::draw();
    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

} // namespace graphicEngine::gl