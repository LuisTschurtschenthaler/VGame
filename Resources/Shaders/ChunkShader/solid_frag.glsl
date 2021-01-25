#version 440 core
out vec4 color;

in vec2 vs_texCoord;
in vec3 vs_normal;
in float vs_ambientOcclusion;

uniform sampler2D textureAtlas;


void main() {
	vec4 textureColor = texture(textureAtlas, vs_texCoord.xy);
	if(textureColor.a == 0.f) discard;

	vec3 result = textureColor.rgb;
	result = mix(result, vec3(0.05f), vs_ambientOcclusion * 0.3f * distance(vs_texCoord, vec2(0.5f)));
	color = (vec4(result, textureColor.a));
}
