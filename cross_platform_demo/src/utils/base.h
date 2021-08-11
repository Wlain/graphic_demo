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
#include <vulkan/vulkan.h>

// clang-format on
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

#define COMMON_CREATE_FUNC(cls, funcName)      \
    static inline cls* create()                \
    {                                          \
        auto* instance = new cls();            \
        if (!instance->funcName())             \
        {                                      \
            delete instance;                   \
            instance = nullptr;                \
            printf("create %s failed!", #cls); \
        }                                      \
        return instance;                       \
    }

/// VK_CHECK macros
#define VK_CHECK(expression)                                                              \
    do                                                                                    \
    {                                                                                     \
        VkResult error = expression;                                                      \
        if (error)                                                                        \
        {                                                                                 \
            RAS_ERROR("VkResult: (%d) :{}: Vulkan Assertion Failed", error, #expression); \
        }                                                                                 \
    } while (0)

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*x))

#define CGE_ENUM_ALIAS(name, member) \
    constexpr auto name##_##member = name::member
#include "logger.h"
#endif // C___BASE_H
