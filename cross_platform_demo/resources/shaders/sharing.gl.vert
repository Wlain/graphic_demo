#version 410 core

uniform mat4 MVP;
layout(location = 0) in vec2 vPos;
out vec2 texcoord;
void main()
{
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    texcoord = vPos;
}
