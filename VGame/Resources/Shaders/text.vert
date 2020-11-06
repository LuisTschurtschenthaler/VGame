#version 440 core

layout (location = 0) in vec4 vertex_position;

out vec2 vs_texCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex_position.xy, 0.f, 1.f);
    vs_texCoords = vertex_position.zw;
}