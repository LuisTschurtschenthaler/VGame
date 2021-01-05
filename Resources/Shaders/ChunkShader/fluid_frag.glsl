#version 440 core
out vec4 color;

in vec3 vs_position;
in vec2 vs_texCoord;
in vec3 vs_normal;
in float vs_ambientOcclusion;

uniform sampler2D textureAtlas;


void main() {
	vec4 textureColor = texture(textureAtlas, vs_texCoord.xy);
	if(textureColor.a == 0.f) discard;

	float ambient = clamp((vs_ambientOcclusion + 1.f) / 4.f, 0.5f, 1.f);

	vec3 result = (textureColor.rgb * (ambient));
	color = (vec4(result, textureColor.a));
}
