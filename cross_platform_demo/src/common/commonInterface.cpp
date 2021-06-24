//
// Created by william on 2021/4/5.
//

#include "commonInterface.h"
#include "utils.h"
namespace graphicEngine
{
CommonInterface::CommonInterface() = default;

CommonInterface::~CommonInterface()
{
    std::cout << "~CommonInterface called" << std::endl;
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void CommonInterface::errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void CommonInterface::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void CommonInterface::initWithProperty(const char* title)
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(640, 480, title, nullptr, nullptr);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwMakeContextCurrent(m_window);
    if (glewInit() != GLEW_OK)
    {
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
}

void CommonInterface::initWithProperty(const std::tuple<const char*, std::string, std::string>& property)
{
    initWithProperty(std::get<0>(property));
    // init program
    m_program = std::make_unique<graphicEngine::Program>(m_externPath + std::get<1>(property), m_externPath + std::get<2>(property));
}

void CommonInterface::update(float elapseTime)
{
}

void CommonInterface::resize(int width, int height)
{
}

void CommonInterface::display()
{
}

void CommonInterface::draw()
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    if (width != m_currentWidth || height != m_currentHeight)
    {
        resize(width, height);
        m_currentWidth = width;
        m_currentHeight = height;
    }
    while (!glfwWindowShouldClose(m_window))
    {
        update((float)glfwGetTime());
        display();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

} // namespace graphicEngine