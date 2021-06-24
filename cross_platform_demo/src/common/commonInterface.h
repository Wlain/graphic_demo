//
// Created by william on 2021/4/5.
//

#ifndef CROSS_PLATFORM_DEMO_COMMONINTERFACE_H
#define CROSS_PLATFORM_DEMO_COMMONINTERFACE_H

#include "base.h"
#include "program.h"

namespace graphicEngine
{
class CommonInterface
{
public:
    CommonInterface();
    virtual ~CommonInterface();

public:
    virtual void initialize() = 0;
    virtual void update(float elapseTime);
    virtual void resize(int width, int height);
    virtual void display();
    virtual void draw();
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

protected:
    void initWithProperty(const std::tuple<const char*, std::string, std::string>& property);
    void initWithProperty(const char* title);

protected:
    GLFWwindow* m_window = nullptr;
    std::unique_ptr<graphicEngine::Program> m_program;
    std::string m_externPath = "/Users/william/git/learning/graphic_demo/cross_platform_demo/resources/shaders/";
    GLuint m_vbo = 0;
    GLuint m_vao = 0;
    int m_currentWidth = 0;
    int m_currentHeight = 0;
};
} // namespace graphicEngine

#endif //CROSS_PLATFORM_DEMO_COMMONINTERFACE_H
