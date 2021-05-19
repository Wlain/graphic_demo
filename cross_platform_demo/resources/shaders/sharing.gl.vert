#version 330 core

uniform mat4 MVP;
layout(location = 0)  attribute vec2 vPos;
varying vec2 texcoord;
void main()
{
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    texcoord = vPos;
}
