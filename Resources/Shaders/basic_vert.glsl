#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texCoords;

out vec2 vs_texCoords;


void main() {
    vs_texCoords = vertex_texCoords;
    gl_Position = vec4(vertex_position, 1.f);
}
