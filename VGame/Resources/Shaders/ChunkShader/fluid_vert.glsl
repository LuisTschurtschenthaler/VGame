#version 440 core

#extension GL_NV_gpu_shader5			: enable
#extension GL_EXT_shader_16bit_storage	: enable
#extension GL_EXT_shader_8bit_storage	: enable

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texCoords;
layout(location = 3) in float vertex_ambientOcclusion;


out vec3 vs_position;
out vec2 vs_texCoord;
out vec3 vs_normal;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraPosition;
uniform float dayTime;

const float PI = 3.14159265359;
const float AMPLITUDE = 0.01;

float getHeight() {
	float h1 = sin(2.f * PI * dayTime + (vertex_position.x * 16.f)) * AMPLITUDE;
    float h2 = sin(2.f * PI * dayTime + (vertex_position.y + vertex_position.x * 8.f)) * AMPLITUDE;
    return (h1 + h2);
}

void main() {
	vs_position = normalize(vertex_position);
	vs_texCoord = vertex_texCoords;
	vs_normal = vertex_normal;

	vec3 worldPos = vec3(vertex_position.x, getHeight(), vertex_position.z);
	gl_Position = projection * view * vec4(normalize(worldPos), 1.f);
}
