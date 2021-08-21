cmake_minimum_required(VERSION 3.17)
project(cross_platform_demo)

set(USE_VULKAN ON)
set(USE_OPENGL OFF)
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
        ${EXTERNAL_DIR}/SOIL2
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
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm/glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glew/build/cmake)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/volk)

if (USE_VULKAN)
    add_executable(cross_platform_demo ${SOURCES} main_vk.cpp)
elseif (USE_OPENGL)
    add_executable(cross_platform_demo ${SOURCES} main_gl.cpp)
elseif (USE_METAL)
    add_executable(cross_platform_demo ${SOURCES} main_mtl.mm)
elseif (USE_OPENCV)
    add_executable(cross_platform_demo ${SOURCES} main_cv.cpp)
endif ()

target_link_libraries(cross_platform_demo
        glfw
        glew
        volk_headers
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

#INCLUDE_DIRECTORIES("/usr/local/include")   #brew安装头文件的目录 /usr/local/include, 包括GLFW的头文件
#INCLUDE_DIRECTORIES("$ENV{VULKAN_SDK}/include")   # VULKAN 的头文件
#LINK_LIBRARIES("/usr/local/Cellar/glfw/3.3.4/lib/libglfw.3.3.dylib")  # GLFW的动态链接库。brew安装链接库的目录 /usr/local/lib
#LINK_LIBRARIES("$ENV{VULKAN_SDK}/lib/libvulkan.1.dylib")  # vulkan 的动态链接库
#LINK_LIBRARIES("$ENV{VULKAN_SDK}/lib/libvulkan.1.2.182.dylib")  #

target_compile_options(cross_platform_demo PUBLIC "-fobjc-arc")