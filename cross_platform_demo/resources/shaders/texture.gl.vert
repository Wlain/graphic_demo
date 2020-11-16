#version 330 core
in vec3 a_position;
in vec2 a_texCoord;
out vec2 v_texCoord;

void main()
{
    v_texCoord = a_texCoord;
    gl_Position = vec4(a_position, 1.0);
}
