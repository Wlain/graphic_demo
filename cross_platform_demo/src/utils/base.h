//
// Created by william on 2020/7/1.
//

#ifndef C___BASE_H
#define C___BASE_H

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <glfw/deps/glad/gl.h>
#include <glfw/deps/linmath.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <typeinfo>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

/// assert macros
#define RAS_ASSERT(expression) assert(expression)

/// Error macro
#ifdef RAS_ERRORS_AS_WARNINGS
    #define RAS_ERROR RAS_WARN
#else
    #define RAS_ERROR(...)                                  \
        do                                                  \
        {                                                   \
            Logger::log(Logger::Level::Error, __VA_ARGS__); \
            RAS_ASSERT(0);                                  \
        } while (0)
#endif

/// Info macro
#define RAS_INFO(...)                                  \
    do                                                 \
    {                                                  \
        Logger::log(Logger::Level::Info, __VA_ARGS__); \
    } while (0)

/// Warn macro
#define RAS_WARN(...)                                  \
    do                                                 \
    {                                                  \
        Logger::log(Logger::Level::Warn, __VA_ARGS__); \
    } while (0)

#endif // C___BASE_H
