#version 440 core

#extension GL_NV_gpu_shader5			: enable
#extension GL_EXT_shader_16bit_storage	: enable
#extension GL_EXT_shader_8bit_storage	: enable

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texCoords;


out vec3 vs_position;
out vec2 vs_texCoord;
out vec3 vs_normal;

uniform mat4 projectionView;
uniform float dayTime;


vec3 getWorldPosition() {
	vec3 inVert = vertex_position;
    inVert.y -= 0.2;
    inVert.y -= sin((dayTime + inVert.z) * 1.2f) / 10.f;
    inVert.y += cos((dayTime + inVert.x) * 1.2f) / 8.f;
    return inVert;
}

void main() {
	vs_position = normalize(vertex_position);
	vs_texCoord = vertex_texCoords;
	vs_normal = vertex_normal;

	gl_Position = projectionView * vec4(getWorldPosition(), 1.f);
}
