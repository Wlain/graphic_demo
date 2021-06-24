//
// Created by william on 2021/6/24.
//

#include "showGLInfo.h"

namespace graphicEngine::gl
{
ShowGLInfo::~ShowGLInfo() = default;

void ShowGLInfo::initialize()
{
    initWithProperty(std::make_tuple("showGLInfo", "", ""));
    int size;
    //float size[2];
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &size);
    std::cout << "GL_MAX_VERTEX_ATTRIBS:" << size << std::endl;
    glGetIntegerv(GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &size);
    std::cout << "GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:" << size << std::endl;
    glGetIntegerv(GL_MAX_TEXTURE_COORDS, &size);
    std::cout << "GL_MAX_TEXTURE_COORDS:" << size << std::endl;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &size);
    std::cout << "GL_MAX_TEXTURE_IMAGE_UNITS:" << size << std::endl;
    glGetIntegerv(GL_FRAGMENT_SHADER, &size);
    std::cout << "GL_FRAGMENT_SHADER:" << size << std::endl;
    glGetIntegerv(GL_VERTEX_SHADER, &size);
    std::cout << "GL_VERTEX_SHADER:" << size << std::endl;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &size);
    std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS:" << size << std::endl;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &size);
    std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS:" << size << std::endl;
    glGetIntegerv(GL_MAX_VARYING_FLOATS, &size);
    std::cout << "GL_MAX_VARYING_FLOATS:" << size << std::endl;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &size);
    std::cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:" << size << std::endl;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &size);
    std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:" << size << std::endl;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &size);
    std::cout << "GL_MAX_TEXTURE_IMAGE_UNITS:" << size << std::endl;
}

void ShowGLInfo::update(float elapseTime)
{
    CommonInterface::update(elapseTime);
}

void ShowGLInfo::resize(int width, int height)
{
    CommonInterface::resize(width, height);
}

void ShowGLInfo::display()
{
    CommonInterface::draw();
}
}