#version 330 core

in vec2 v_texCoord;
uniform sampler2D u_texture;
out vec4 fragColor;

void main()
{
    vec3 texture = texture(u_texture, vec2(v_texCoord.x, 1.0 - v_texCoord.y)).rgb;
    fragColor = vec4(texture, 1.0);
}
