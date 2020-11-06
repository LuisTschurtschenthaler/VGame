#version 440 core

#extension GL_NV_gpu_shader5			: enable
#extension GL_EXT_shader_16bit_storage	: enable
#extension GL_EXT_shader_8bit_storage	: enable

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_uvWithTextureID;


out vec3 vs_position;
out vec2 vs_texCoord;
out vec3 vs_normal;

uniform mat4 projectionView;
uniform int atlasSize;
uniform int tileSize;


const vec3 ToD[7] = vec3[7](vec3(0.58597,0.15,0.02),
							vec3(0.58597,0.35,0.09),
							vec3(0.58597,0.5,0.26),
							vec3(0.58597,0.5,0.35),
							vec3(0.58597,0.5,0.36),
							vec3(0.58597,0.5,0.37),
							vec3(0.58597,0.5,0.38));

//const vec2 texCoords[4] = vec2[4](
//	vec2(0.f, 0.f),
//	vec2(1.f, 0.f),
//	vec2(0.f, 1.f),
//	vec2(1.f, 1.f)
//);
//const vec3 normals[6] = vec3[6] (
//	vec3( 1.f, 0.f, 0.f),
//	vec3(-1.f, 0.f, 0.f),
//	vec3(0.f,  1.f, 0.f),
//	vec3(0.f, -1.f, 0.f),
//	vec3(0.f, 0.f,  1.f),
//	vec3(0.f, 0.f, -1.f)
//);

vec2 calculateTextureCoordinates() {
	float x = mod(vertex_uvWithTextureID.z, tileSize);
	float y = floor(vertex_uvWithTextureID.z / tileSize);

	if(vertex_uvWithTextureID.xy == vec2(0.f, 0.f))
		return vec2((x * tileSize / atlasSize), (y * tileSize / atlasSize));

	else if(vertex_uvWithTextureID.xy == vec2(1.f, 0.f))
		return vec2(((x + 1) * tileSize / atlasSize), (y * tileSize / atlasSize));

	else if(vertex_uvWithTextureID.xy == vec2(1.f, 1.f))
		return  vec2(((x + 1) * tileSize / atlasSize), ((y + 1) * tileSize / atlasSize));

	else if(vertex_uvWithTextureID.xy == vec2(0.f, 1.f))
		return  vec2((x * tileSize / atlasSize), ((y + 1) * tileSize / atlasSize));
}


void main() {
	vs_position = normalize(vertex_position);
	vs_texCoord = calculateTextureCoordinates();
	vs_normal = vertex_normal;

	gl_Position = projectionView * vec4(vertex_position, 1.f);
}
