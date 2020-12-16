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
out float vs_ambientOcclusion;

uniform mat4 projectionView;


void main() {
	vs_position = normalize(vertex_position);
	vs_texCoord = vertex_texCoords;
	vs_normal = vertex_normal;
	vs_ambientOcclusion = vertex_ambientOcclusion;

	gl_Position = projectionView * vec4(vertex_position, 1.f);
}
