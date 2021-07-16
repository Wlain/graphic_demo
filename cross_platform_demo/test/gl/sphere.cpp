//
// Created by william on 2021/6/25.
//

#include "sphere.h"

namespace graphicEngine::gl
{
void sphere::initialize()
{
}
sphere::~sphere()
{
}
void sphere::update(float elapseTime)
{
    CommonInterface::update(elapseTime);
}
void sphere::resize(int width, int height)
{
    CommonInterface::resize(width, height);
}
void sphere::display()
{
    CommonInterface::display();
}
}