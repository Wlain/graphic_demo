#version 330 core

uniform sampler2D texture;
uniform vec3 color;
varying vec2 texcoord;
void main()
{
    gl_FragColor = vec4(color * texture2D(texture, texcoord).rgb, 1.0);
}