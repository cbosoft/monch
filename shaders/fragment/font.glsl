#version 330 core

in vec4 rgba;
in vec2 tex_coords;

uniform sampler2D screenTexture;

out vec4 color;

void main()
{
    vec4 sampled = texture(screenTexture, tex_coords);
    color = vec4(rgba.xyz, sampled.x+sampled.y+sampled.z);
}
