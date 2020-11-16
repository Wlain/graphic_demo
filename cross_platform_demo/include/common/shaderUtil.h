//
// Created by william on 2020/11/16.
//

#ifndef CROSS_PLATFORM_DEMO_SHADERUTIL_H
#define CROSS_PLATFORM_DEMO_SHADERUTIL_H

//
//  shader.hpp
//  LearnOpenGL
//
//  Created by william on 2020/4/9.
//  Copyright © 2020 william. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include "base.h"

namespace GraphicEngine
{
class ShaderUtil
{
public:
    ShaderUtil() = default;
    ~ShaderUtil() = default;
    ShaderUtil(const ShaderUtil& s) = default;
    ShaderUtil(ShaderUtil&& a) = default;
    ShaderUtil& operator=(const ShaderUtil& s) = default;
    ShaderUtil& operator=(ShaderUtil&& s) = default;
    GLuint getProgram() const;
    ShaderUtil(const char* vertexPath, const char* fragmentPath);
    static GLuint CreateProgram(GLuint vertShader, GLuint fragShader);
    static GLuint CompileShader(GLenum shaderType, const GLchar* src);
    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVector2(const std::string& name, const glm::vec2& value) const;
    void setVector2(const std::string& name, float x, float y) const;
    void setVector3(const std::string& name, const glm::vec3& value) const;
    void setVector3(const std::string& name, float x, float y, float z) const;
    void setVector4(const std::string& name, const glm::vec4& value) const;
    void setVector4(const std::string& name, float x, float y, float z, float w) const;
    void setMatrix2(const std::string& name, const glm::mat2& mat) const;
    void setMatrix3(const std::string& name, const glm::mat3& mat) const;
    void setMatrix4(const std::string& name, const glm::mat4& mat) const;

private:
    GLuint mProgram;
};
}
#endif /* shader_hpp */


#endif //CROSS_PLATFORM_DEMO_SHADERUTIL_H
