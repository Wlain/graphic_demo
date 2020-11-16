cmake_minimum_required(VERSION 3.17)
project(cross_platform_demo)

set(USE_VULKAN OFF)
set(USE_OPENGL ON)
set(USE_METAL OFF)
set(USE_OPENCV OFF)

#set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
#set(CMAKE_SUPPRESS_REGENERATION ON)

# 设置用debug还是release模式。debug允许断点，而release更快
#set(CMAKE_BUILD_TYPE Debug)
# 允许c++11标准、O3优化、多线程。match选项可避免一些cpu上的问题
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread")
set(CMAKE_CXX_STANDARD 20)
#
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXTERNAL_DIR ${ROOT_DIR}/3rdparty)

# vulkan
set(VK_SDK_DIR ${EXTERNAL_DIR}/MoltenVK)
# opencv
find_package(OpenCV)

list(APPEND EXTERNAL_DIR
        ${EXTERNAL_DIR}/stb/include
        ${EXTERNAL_DIR}/glm
        ${EXTERNAL_DIR}/glad/include
        ${EXTERNAL_DIR}/egl
        ${VK_SDK_DIR}
        ${VK_SDK_DIR}/include
        ${EXTERNAL_DIR}MoltenVK
        ${EXTERNAL_DIR}MoltenVK/include
        ${OpenCV_INCLUDE_DIRS}
        )

file(GLOB_RECURSE EXTERNAL_SRC
        3rdparty/stb/src/*.cpp
        3rdparty/glad/src/*.c
        3rdparty/glew/*.cpp
        3rdparty/egl/*.cpp
        )

# 头文件目录
include_directories(
        ${EXTERNAL_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/core
        ${PROJECT_SOURCE_DIR}/include/common
        ${PROJECT_SOURCE_DIR}/include/utils
)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# 源文件目录
file(GLOB SOURCES
        ${EXTERNAL_SRC}
        include/*.h
        include/core/*.h
        src/core/*.cpp
        include/utils/*.h
        include/common/*.h
        src/common/*.cpp
        src/utils/*.cpp
        src/core/*.mm
        src/*.cpp
        test/*.cpp
        )

file(GLOB_RECURSE VK_STATIC_LIB "${VK_SDK_DIR}/macOS/lib/*.a")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glfw)

if (USE_VULKAN)
    add_executable(cross_platform_demo ${SOURCES} main_vk.cpp)
elseif(USE_OPENGL)
    add_executable(cross_platform_demo ${SOURCES} main_gl.cpp)
elseif(USE_METAL)
    add_executable(cross_platform_demo ${SOURCES} main_mtl.mm)
elseif(USE_OPENCV)
    add_executable(cross_platform_demo ${SOURCES} main_cv.cpp)
endif ()

target_link_libraries(cross_platform_demo
        glfw
        ${VK_STATIC_LIB}
        ${OpenCV_LIBS}
        ${VK_SDK_DIR}/macOS/MoltenVK.framework/MoltenVK
        "-framework OpenGL"
        "-framework GLUT"
        "-framework Metal"
        "-framework MetalKit"
        "-framework QuartzCore"
        "-framework CoreMedia"
        "-framework AVFoundation"
        "-framework Accelerate"
        "-framework IOSurface"
        )
target_compile_options(cross_platform_demo PUBLIC "-fobjc-arc")