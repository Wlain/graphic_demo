//
// Created by william on 2020/11/16.
//

#include "program.h"

namespace graphicEngine
{
GLuint Program::CreateProgram(GLuint vertShader, GLuint fragShader)
{
    GLuint program = 0;
    GLint linked = GL_FALSE;
    program = glCreateProgram();
    if (!program)
    {
        goto exit;
    }
    // 绑定
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    // 解绑定
    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        printf("could not link program\n");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen)
        {
            auto infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog)
            {
                glGetProgramInfoLog(program, infoLogLen, nullptr, infoLog);
                printf("could not link program\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }
    return program;
exit:
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return program;
}

GLuint Program::CompileShader(GLenum shaderType, const GLchar* src)
{
    GLuint shader = glCreateShader(shaderType);
    if (!shader)
    {
        return 0;
    }
    glShaderSource(shader, 1, &src, nullptr);
    GLint compiled = GL_TRUE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        GLsizei infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0)
        {
            auto* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog)
            {
                glGetShaderInfoLog(shader, infoLogLen, nullptr, infoLog);
                printf("Cound not compile %s shader:\n%s\nshader Code: \n%s\n",
                       shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog, src);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

Program::Program(const std::string_view& vertexPath, const std::string_view& fragmentPath)
{
    // 1. 从文件中读取顶点和片元的文本文件
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 确保ifstream objects会抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::Program::file not successfully read" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vShaderCode);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fShaderCode);
    mProgram = CreateProgram(vertexShader, fragmentShader);
}

GLuint Program::getProgram() const
{
    return mProgram;
}

void Program::use() const
{
    glUseProgram(mProgram);
}

void Program::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(mProgram, name.c_str()), (int)value);
}

void Program::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(mProgram, name.c_str()), value);
}

void Program::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(mProgram, name.c_str()), value);
}

void Program::setVector2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
}

void Program::setVector2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(mProgram, name.c_str()), x, y);
}

void Program::setVector3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
}

void Program::setVector3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(mProgram, name.c_str()), x, y, z);
}

void Program::setVector4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
}

void Program::setVector4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(mProgram, name.c_str()), x, y, z, w);
}

void Program::setMatrix2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Program::setMatrix3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Program::setMatrix4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
} // namespace graphicEngine
