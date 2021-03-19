#version 440 core

layout(location = 0) in uvec2 vertex;

out vec3 vs_position;
out vec2 vs_texCoord;
out float vs_lightLevel;

uniform vec3 chunkWorldCoord;
uniform mat4 view;
uniform mat4 projection;


const vec2 TEXTURE_COORDS[4] = vec2[4](
	vec2(0.f, 0.f),
	vec2(1.f, 0.f),
	vec2(0.f, 1.f),
	vec2(1.f, 1.f)
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


void main() {
	float x = float(vertex[0] & 0xFF);
	float y = float((vertex[0] & 0xFF00) >> 8);
	float z = float((vertex[0] & 0xFF0000) >> 16);
	
	float lightLevel = float((vertex[0] & 0x0F000000) >> 24) / 5.f;
	
	uint textureID = uint(vertex[1] & 0xFF);
    uint index	   = uint((vertex[1] & 0xF00) >> 8);
	
	vs_texCoord = getTextureCoordinates(index, textureID);
	vs_lightLevel = lightLevel;

	vs_position = vec3(x, y, z) + chunkWorldCoord;
	gl_Position = projection * view * vec4(vs_position, 1.f);
}
