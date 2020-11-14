cmake_minimum_required(VERSION 3.16)
project(cross_platform_demo)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_SUPPRESS_REGENERATION ON)

# 设置用debug还是release模式。debug允许断点，而release更快
set(CMAKE_BUILD_TYPE Debug)
# 允许c++11标准、O3优化、多线程。match选项可避免一些cpu上的问题
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread")
set(CMAKE_CXX_STANDARD 20)
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXTERNAL_HEADERS ROOT_DIR/3rdparty)

# glew glfw
set(GLEW_H /usr/local/Cellar/glew/2.1.0_1/include)
set(GLFW_H /usr/local/Cellar/glfw/3.3.2/include)
set(GLAD_H ${EXTERNAL_HEADERS}/glad/include)

# opencv
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

list(APPEND EXTERNAL_HEADERS
        ${GLEW_H}
        ${GLFW_H}
        ${EXTERNAL_HEADERS}/egl
        3rdparty/stb
        3rdparty/glad/include
        3rdparty/glm
        )

file(GLOB_RECURSE EXTERNAL_SRC
        3rdparty/stb/*.cpp
        3rdparty/glad/src/*.c
        )

# 头文件目录
include_directories(
        ${EXTERNAL_HEADERS}
        ${PROJECT_SOURCE_DIR}/include
)

# 源文件目录
file(GLOB SOURCES
        EXTERNAL_SRC
        3rdparty/glad/src/glad.c
        include/*.h
        include/core/*.h
        src/core/*.cpp
        include/utils/*.h
        src/utils/*.cpp
        src/core/*.mm
        src/*.cpp
        test/*.cpp
        *.cpp
        )

set(GLEW_LINK /usr/local/Cellar/glew/2.1.0_1/lib/libGLEW.2.1.0.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.2/lib/libglfw.3.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

add_executable(cross_platform_demo ${SOURCES} main.cpp)

target_link_libraries(cross_platform_demo
        "-framework OpenGL"
        "-framework GLUT"
        "-framework Metal"
        "-framework MetalKit"
        "-framework QuartzCore"
        "-framework CoreMedia"
        "-framework AVFoundation"
        )
target_compile_options(cross_platform_demo PUBLIC "-fobjc-arc")

target_link_libraries(cross_platform_demo ${OpenCV_LIBS})