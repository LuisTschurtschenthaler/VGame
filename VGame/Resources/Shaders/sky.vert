#version 440 core

layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertex_normal;

out vec3 vs_pos;

uniform mat4 mvp;


void main() {
	vs_pos = normalize(vertex_position);
    
	vec4 pos = mvp * vec4(vertex_position, 1.f);
	gl_Position = pos.xyww;
}
