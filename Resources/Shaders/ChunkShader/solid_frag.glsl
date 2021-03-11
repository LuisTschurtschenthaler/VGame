#version 440 core
out vec4 color;

in vec3 vs_position;
in vec3 vs_normal;
in vec2 vs_texCoord;
in float vs_ambientOcclusion;

uniform sampler2D textureAtlas;
uniform vec3 playerPosition;
uniform int renderDistance;


float getFogFactor() {
	float FOG_MAX = renderDistance - 15.f;
	float FOG_MIN = FOG_MAX - 20.f;

	float dist = distance(playerPosition, vs_position);
	return ((dist - FOG_MIN) / (FOG_MAX - FOG_MIN));
}


void main() {
	vec4 textureColor = texture(textureAtlas, vs_texCoord.xy);

	float fogFactor = getFogFactor();
	fogFactor = clamp(fogFactor, 0.f, 1.f);

	if(textureColor.a == 0.f || fogFactor >= 1.f) discard;

	vec3 result = textureColor.rgb;
	result = mix(result, vec3(0.05f), vs_ambientOcclusion * 0.3f * distance(vs_texCoord, vec2(0.5f)));
	color = (vec4(result, (textureColor.a - fogFactor)));
}
