#version 410 core

uniform sampler2D inputTexture;
uniform vec3 color;
in vec2 texcoord;
out vec4 outColor;
void main()
{
    outColor = vec4(color * texture(inputTexture, texcoord).rgb, 1.0);
}