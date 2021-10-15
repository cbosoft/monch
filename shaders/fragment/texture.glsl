#version 330 core

in vec4 rgba;
in vec2 tex_coords;

out vec4 color;

uniform sampler2D screenTexture;

void main()
{
    color = texture(screenTexture, tex_coords)*rgba;
}
