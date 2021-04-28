#version 440 core

#define WATER	0
#define LAVA	1
#define CACTUS	13


layout(location = 0) in uvec2 vertex;

out vec3 vs_position;
out vec2 vs_texCoord;
out float vs_lightLevel;
out float vs_ao;

uniform vec3 chunkWorldCoord;
uniform mat4 view;
uniform mat4 projection;


const float OFFSET = (1.f / 16.f);

const vec2 TEXTURE_COORDS[4] = vec2[4](
	vec2(0.f, 0.f),
	vec2(1.f, 0.f),
	vec2(0.f, 1.f),
	vec2(1.f, 1.f)
);

const vec3 NORMALS[7] = vec3[7](
	vec3( 1.f, 0.f, 0.f),
	vec3(-1.f, 0.f, 0.f),
	vec3(0.f,  1.f, 0.f),
	vec3(0.f, -1.f, 0.f),
	vec3(0.f, 0.f,  1.f),
	vec3(0.f, 0.f, -1.f),
	vec3(0.f, 0.f,  0.f) // NONE
);


vec2 getTextureCoordinates(uint index, uint textureID) {
	float x = textureID % 16;
	float y = floor(textureID / 16);

	vec2 uv = TEXTURE_COORDS[index];

	if(uv == vec2(0.f, 0.f)) return vec2((x / 16), (y / 16));
	else if(uv == vec2(1.f, 0.f)) return vec2((x + 1.f) / 16, (y / 16));
	else if(uv == vec2(1.f, 1.f)) return vec2(((x + 1.f) / 16), (y + 1.f) / 16);
	else if(uv == vec2(0.f, 1.f)) return vec2((x / 16), (y + 1.f) / 16);
	return vec2(0.f);
}

vec3 getVertexOffset(uint textureID, vec3 normal) {
	vec3 vertexOffset = vec3(0.f);
	
	if(textureID == WATER || textureID == LAVA) {
		if(normal == NORMALS[2])
			vertexOffset.y += OFFSET;
	}
	
	else if(textureID == CACTUS) {
		if(normal == NORMALS[0])
			vertexOffset.x += OFFSET;
		if(normal == NORMALS[1])
			vertexOffset.x -= OFFSET;
		
		if(normal == NORMALS[4])
			vertexOffset.z += OFFSET;
		if(normal == NORMALS[5])
			vertexOffset.z -= OFFSET;
	}

	return vertexOffset;
}


void main() {
	float x = float(vertex[0] & 0xFF);
	float y = float((vertex[0] & 0xFF00) >> 8);
	float z = float((vertex[0] & 0xFF0000) >> 16);
	
	float lightLevel = float((vertex[0] & 0x0F000000) >> 24) / 5.f;
	
	uint textureID	   = uint(vertex[1] & 0xFF);
    uint textureCoords = uint((vertex[1] & 0xFF00) >> 8);
	uint normalIndex   = uint((vertex[1] & 0xFF0000) >> 16);
	
	float ao		   = float((vertex[1] & 0xFF000000) >> 24);
	
	vs_texCoord = getTextureCoordinates(textureCoords, textureID);
	vs_lightLevel = lightLevel;
	vs_ao = ao;

	vec3 vertexOffset = getVertexOffset(textureID, NORMALS[normalIndex]);
	vs_position = (vec3(x, y, z) - vertexOffset) + chunkWorldCoord;
	gl_Position = projection * view * vec4(vs_position, 1.f);
}
