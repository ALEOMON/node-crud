#version 420

uniform mat4 mvp_matrix;

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec2 _uv;

out vec2 uv;

void main() {
  gl_Position =