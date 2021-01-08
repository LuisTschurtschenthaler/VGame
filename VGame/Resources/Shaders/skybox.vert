#version 440 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_sunPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 sunPosition;


void main() {
	vs_position = vec3(model * vec4(vertex_position, 1.f));
	vs_sunPosition = normalize(sunPosition);

	vec4 pos = projection * view * model * vec4(vertex_position, 1.f);
	gl_Position = pos;
}