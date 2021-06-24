//
// Created by william on 2021/6/23.
//

#include "background.h"

namespace graphicEngine::gl
{
Background::~Background() = default;

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

void Background::display()
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
} // namespace graphicEngine::gl