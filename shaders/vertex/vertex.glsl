#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 tex_coords_i;

out vec4 rgba;
out vec2 tex_coords;

void main() {
  gl_Position = vec4(pos, 1.0f);
  rgba = colour;
  tex_coords = tex_coords_i;
}
