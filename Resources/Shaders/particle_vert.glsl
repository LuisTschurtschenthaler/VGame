#version 440 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texCoord;

out vec2 vs_texCoord;

uniform mat4 projection;
uniform mat4 view;


void main() {
	vs_texCoord = vertex_texCoord;
	gl_Position = projection * view * vec4(vertex_position, 1.f);
}

