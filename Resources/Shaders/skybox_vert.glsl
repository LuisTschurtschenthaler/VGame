#version 440 core
layout(location = 0) in vec3 vertex_position;

out vec3 vs_position;

uniform mat4 projection;
uniform mat4 view;


void main() {
	vs_position = normalize(vertex_position);

	vec4 position = projection * view * vec4(vertex_position, 1.f);
	gl_Position = position.xyww;
}
