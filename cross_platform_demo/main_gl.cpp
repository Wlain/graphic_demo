//
// Created by william on 2020/11/14.
//

#include "base.h"
#include "fileSystem.h"
#include "shaderUtil.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

enum AttribLocation
{
    Positions,
    TexCoords,
    NumAttribs
};

enum UniformLocation
{
    NumUniforms
};

GLint uniforms[NumUniforms];
GLint attribs[NumAttribs];

unsigned int s_width = 640;
unsigned int s_height = 852;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    auto window = glfwCreateWindow(s_width, s_height, "opengl window", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to Create OpenGL Context" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    GraphicEngine::ShaderUtil shader("resources/shaders/texture.gl.vert", "resources/shaders/texture.gl.frag");
    static float vertices[] = {
        // positions        // texCoords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f   // bottom right
    };
    static unsigned short indices[] = {
        0, 1, 2, 3
    };
    GLuint vbo, vao, ebo;
    attribs[Positions] = glGetAttribLocation(shader.getProgram(), "a_position");
    attribs[TexCoords] = glGetAttribLocation(shader.getProgram(), "a_texCoord");
    // mac 4.0 以后需要绑定vao
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(attribs[Positions], 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attribs[Positions]);
    glVertexAttribPointer(attribs[TexCoords], 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(attribs[TexCoords]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // load and create a texture0
    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    GLubyte* data = stbi_load(GraphicEngine::FileSystem::getPath("resources/tianlei.jpg").c_str(), &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // 告诉opengl需要使用哪个纹理单元
    shader.use();
    glUniform1i(glGetUniformLocation(shader.getProgram(), "u_texture"), 0);
    // mvp
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)s_width/(float)s_height, 0.1f, 100.0f);
    glm::mat4 mvpMatrix = projection * view * model;
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "u_mvpMatrix"), 1, GL_FALSE, (const float *)glm::value_ptr(mvpMatrix));
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
        // glfw:swap buffer and poll IO events(keys pressed/released, mouse move etc.)
        glfwSwapBuffers(window);
        glfwPollEvents(); // 检查有没有触发什么事件
    }
    glfwTerminate();
    return 0;
}