//
// Created by william on 2021/5/18.
//

#ifndef CROSS_PLATFORM_DEMO_TRIANGLE_VK_H
#define CROSS_PLATFORM_DEMO_TRIANGLE_VK_H
#include "base.h"

class TriangleVk
{
public:
    void run();

private:
    void createInstance();
    void initVulkan();
    void mainLoop();
    void cleanup();

private:
    GLFWwindow* m_window = nullptr;
    VkInstance  m_instance{};
    const int WIDTH = 800;
    const int HEIGHT = 600;
};

#endif //CROSS_PLATFORM_DEMO_TRIANGLE_VK_H
