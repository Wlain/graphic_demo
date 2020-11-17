#version 330 core
in vec3 a_position;
in vec2 a_texCoord;
uniform mat4 u_mvpMatrix;
uniform vec2 u_scaling;
out vec2 v_texCoord;

void main()
{
    v_texCoord = (a_position.xy * u_scaling + 1.0) / 2.0;
    gl_Position = u_mvpMatrix *  vec4(a_position, 1.0);
}
