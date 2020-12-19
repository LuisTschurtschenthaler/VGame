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

uniform mat4 projection;
uniform mat4 view;

uniform vec3 cameraPosition;
uniform float dayTime;


vec3 getWorldPosition() {
	vec3 inVert = vertex_position;
    inVert.x -= sin((dayTime + inVert.z + inVert.y) * 2.f) / 20.f;
    //inVert.y += 0.01 - (sin((dayTime + inVert.z + inVert.y) * 2.f) / 20.f);
    inVert.y += 0.001;
    inVert.z -= cos((dayTime + inVert.x + inVert.y) * 2.f) / 20.f;
    return inVert;
}


void main() {
	vs_position = normalize(vertex_position);
	vs_texCoord = vertex_texCoords;
	vs_normal = vertex_normal;
	vs_ambientOcclusion	= vertex_ambientOcclusion;

	vec3 worldPos = getWorldPosition();
	vec3 finalVec = (distance(worldPos, cameraPosition) <= 20) ? worldPos : vertex_position;
	gl_Position = projection * view * vec4(finalVec, 1.f);
}
