//
// Created by william on 2020/7/1.
//

#ifndef C___BASE_H
#define C___BASE_H

// clang-format off
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <typeinfo>
#include <vector>
#include <GL/glew.h>
#include <glfw/deps/linmath.h>
#include <GLFW/glfw3.h>
// clang-format on

/// assert macros
#define ASSERT(expression) assert(expression)

/// Error macro
#ifdef ERRORS_AS_WARNINGS
    #define LOG_ERROR LOG_WARN
#else
    #define LOG_ERROR(...)                                  \
        do                                                  \
        {                                                   \
            Logger::log(Logger::Level::Error, __VA_ARGS__); \
        } while (0)
#endif

/// Info macro
#define LOG_INFO(...)                                  \
    do                                                 \
    {                                                  \
        Logger::log(Logger::Level::Info, __VA_ARGS__); \
    } while (0)

/// Warn macro
#define LOG_WARN(...)                                  \
    do                                                 \
    {                                                  \
        Logger::log(Logger::Level::Warn, __VA_ARGS__); \
    } while (0)


#if defined(NDEBUG) && defined(__GNUC__)
    #define ASSERT_ONLY __attribute__((unused))
#else
    #define ASSERT_ONLY
#endif

/// VK_CHECK macros
#define VK_CHECK(expression)                                                              \
    do                                                                                    \
    {                                                                                     \
        VkResult ASSERT_ONLY error = expression;                                          \
        if (error)                                                                        \
        {                                                                                 \
            LOG_ERROR("VkResult: (%d) :{}: Vulkan Assertion Failed", error, #expression); \
        }                                                                                 \
    } while (0)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

#define CGE_ENUM_ALIAS(name, member) \
    constexpr auto name##_##member = name::member

#include "logger.h"
#endif // C___BASE_H
