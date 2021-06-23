cmake_minimum_required(VERSION 3.17)
project(cross_platform_demo)

set(USE_VULKAN OFF)
set(USE_OPENGL ON)
set(USE_METAL OFF)
set(USE_OPENCV OFF)

set(CMAKE_CXX_STANDARD 20)
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXTERNAL_DIR ${ROOT_DIR}/3rdparty)

# vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)

# opencv
find_package(OpenCV)

list(APPEND EXTERNAL_DIR
        ${EXTERNAL_DIR}/glm
        ${EXTERNAL_DIR}/SOIL2
        ${EXTERNAL_DIR}/glfw/deps/glad
        ${EXTERNAL_DIR}/egl
        ${VK_SDK_DIR}
        ${VK_SDK_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
        )

# 头文件目录
include_directories(
        ${EXTERNAL_DIR}
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/core
        ${PROJECT_SOURCE_DIR}/src/common
        ${PROJECT_SOURCE_DIR}/src/utils
        ${PROJECT_SOURCE_DIR}/src/test
        ${PROJECT_SOURCE_DIR}/test/gl
        ${PROJECT_SOURCE_DIR}/test/vk
)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)


file(GLOB_RECURSE EXTERNAL_SRC
        3rdparty/glfw/deps/glad/*.c
        3rdparty/glew/*.cpp
        3rdparty/glfw/deps/*.c
        3rdparty/glfw/deps/*.h
        3rdparty/egl/*.cpp
        3rdparty/SOIL2/*.c
        )

# 源文件目录
file(GLOB_RECURSE SOURCES
        ${EXTERNAL_SRC}
        src/*.h
        src/*.inl
        src/*.cpp
        src/*.mm
        test/*.h
        test/*.cpp
        test/*.inl
        )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glfw)

if (USE_VULKAN)
    add_executable(cross_platform_demo ${SOURCES} main_vk.cpp)
elseif (USE_OPENGL)
    add_executable(cross_platform_demo ${SOURCES} main_gl.cpp)
elseif (USE_METAL)
    add_executable(cross_platform_demo ${SOURCES} main_mtl.mm)
elseif (USE_OPENCV)
    add_executable(cross_platform_demo ${SOURCES} main_cv.cpp src/test/vulkanLayerExternsionDefine.cpp src/test/vulkanLayerExternsionDefine.h src/test/vulkanInstance.cpp src/test/vulkanInstance.h src/test/vulkanApplications.cpp src/test/vulkanApplications.h src/test/vulkanDevice.cpp src/test/vulkanDevice.h test/gl/background.cpp test/gl/background.h test/gl/point.cpp test/gl/point.h)
endif ()

target_link_libraries(cross_platform_demo
        glfw
        Vulkan::Vulkan
        ${VK_STATIC_LIB}
        ${OpenCV_LIBS}
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